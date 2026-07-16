#include "../include/thread-pool/thread-pool.h"

ThreadPool::ThreadPool(size_t nthreads) : numThreads(nthreads)
{
	std::cout << "Creating Threads - "<< numThreads << "\n";
	for (size_t i{ 0 }; i < numThreads; i++) {
		workers.push_back(std::thread([this]() {payload();}));
	}
}

void ThreadPool::payload() { 
	std::cout << "Thread Id: " << std::this_thread::get_id() << "\n";
}

ThreadPool::~ThreadPool() {
	for (auto& thread : workers) {
		if (thread.joinable()) thread.join();
	}
}