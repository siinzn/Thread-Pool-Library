#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <future>
#include "task.h"


class ThreadPool {
public:
	ThreadPool(size_t nthreads);
	void payload();

	// F is just a placeholder for any type - can be int string char etc etc 
	// typename... this says that this a parameter pack meanign there will be zero or multiple params 
	template<typename F, typename... Args>
	auto submit(F&& f, Args&&... args) { 
		// lowercase f and args is just the callable to use inside the submit fucntion
		using ReturnType = std::invoke_result_t<F, Args...>; // this is to get what return type the function is since caller can call any type of functions

		std::packaged_task<ReturnType()> pTask(
			[f = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable{
				return std::invoke(f, args...);
			}
		);

		std::future<ReturnType> result = pTask.get_future(); 
		std::lock_guard<std::mutex> lock(queueMutex);
		auto func = std::make_unique<TaskImpl<std::packaged_task<ReturnType()>>>(std::move(pTask)); // we need to use std::move since packaged_task cannot be copied
		queue.push(std::move(func));
		cv.notify_one();
		return result;
	};

	~ThreadPool();
private:
	size_t numThreads;
	std::vector<std::thread> workers;
	std::queue<std::unique_ptr<TaskBase>> queue; //instead of std::function<> queue we use unique_ptr<TaskBase> since function demands whatever is inside to be copyable and packaged_task isnt
	std::mutex queueMutex;
	std::condition_variable cv;
	bool isStopping = false;
};