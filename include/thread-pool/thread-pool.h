#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <future>


class ThreadPool {
public:
	ThreadPool(size_t nthreads);
	void payload();

	//apparently template functions have to be implemented in header files and not in cpp files. crazy 
	// F is just a placeholder for any type - can be int string char etc etc 
	// typename... this says that this a parameter pack meanign there will be zero or multiple params 
	template<typename F, typename... Args>
	auto submit(F&& f, Args&&... args) { 
		// lowercase f and args is just the callable to use inside the submit fucntion
		using ReturnType = std::invoke_result_t<F, Args...>; // this is to get what return type the function is since caller can call any type of functions
		auto sharedPtr = std::make_shared<std::packaged_task<ReturnType()>>(
			[f = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable {
			return std::invoke(f, args...); // std::invoke lets u call any type of function but how it is called is handled at compile time and we dont have to worry about it.
		});
		std::future<ReturnType> result = sharedPtr->get_future(); // can be written as (*sharedPtr).get_future()

		// lock_guard doesnt really allow to unlock manually, its a strict lock basically
		std::lock_guard<std::mutex> lock(queueMutex);
		std::function<void()> func = [sharedPtr]() mutable { 
			(*sharedPtr)(); //this is a pointer to the packagad task. 
		};
		queue.push(func);
		cv.notify_one();
		return result;
	};

	~ThreadPool();
private:
	size_t numThreads;
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> queue;
	std::mutex queueMutex;
	std::condition_variable cv;
	bool isStopping = false;
};