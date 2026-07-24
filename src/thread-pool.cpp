#include "../include/thread-pool/thread-pool.h"

ThreadPool::ThreadPool(size_t nthreads) : numThreads(nthreads)
{
	std::cout << "Creating Threads - "<< numThreads << "\n";
	// create a new vector of threads & no. of threads depends parameter and runs whatever function is passed on
	for (size_t i{ 0 }; i < numThreads; i++) {
		workers.push_back(std::thread([this]() {payload();}));
	}
}

void ThreadPool::payload() { 
	//std::cout << "Thread Id: " << std::this_thread::get_id() << "\n";
	while (true) {
		//lock so all the threads are in line to get the task from queue
		std::unique_lock<std::mutex> lock(queueMutex);
		//
		cv.wait(lock, [&]() {
			return !queue.empty() || isStopping;
			});

		if (queue.empty() && isStopping) {
			lock.unlock();
			return;
		}

		// if the tasks are not empty - take a task -> pop it -> release the mutex lock -> run that task
		if (!queue.empty()) {
			auto task = queue.front();
			queue.pop();
			lock.unlock();

			// try/catch block catches any exception
			try { task(); }
			catch (const std::exception& e)
			{
				std::cerr << "Caught exception from worker thread: " << e.what() << "\n";
			}
			catch (...) {
				std::cerr << "Caught unkown exception from worker thread: \n";
			}
		}
	}
}
/*
apparently template functions have to be implemented in header files and not in cpp files. crazy 
template void ThreadPool::submit(F f, Args... args) {
	// lock_guard doesnt really allow to unlock manually, its a strict lock basically
	std::lock_guard<std::mutex> lock(queueMutex);
	std::function<void()> func = [a, b]() {
		std::cout << a + b << "\n";
		};
	queue.push(func);
	cv.notify_one();
}
*/

ThreadPool::~ThreadPool() {
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		isStopping = true;
	}

	cv.notify_all();

	for (auto& worker : workers) {
		if (worker.joinable()) worker.join();//bit hard to understand but essentially, the class cannot be destroyed until every worker thread which is active exits
	}
}