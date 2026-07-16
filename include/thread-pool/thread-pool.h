#include <iostream>
#include <vector>
#include <thread>


class ThreadPool {
public:
	ThreadPool(size_t nthreads);
	void payload();
	~ThreadPool();
private:
	size_t numThreads;
	std::vector<std::thread> workers;
};