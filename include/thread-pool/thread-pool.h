#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>


class ThreadPool {
public:
	ThreadPool(size_t nthreads);
	void payload();
	void submit(std::function<void()>);
	~ThreadPool();
private:
	size_t numThreads;
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> queue;
	std::mutex queueMutex;
	std::condition_variable cv;
	bool isStopping = false;
};