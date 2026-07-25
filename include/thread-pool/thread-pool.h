#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <utility>


class ThreadPool {
public:
	ThreadPool(size_t nthreads);
	void payload();

	template<typename F, typename... Args>
	void submit(F&& f, Args&&... args) {
		// lock_guard doesnt really allow to unlock manually, its a strict lock basically
		std::lock_guard<std::mutex> lock(queueMutex);
		std::function<void()> func = [f = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable{
			f(args...);
			};
		queue.push(func);
		cv.notify_one();
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