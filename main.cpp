
#include "./include/thread-pool/thread-pool.h"

void print(int a, std::string yes) {
	std::cout << yes << a <<"\n";
}

int main()
{
	std::cout << "Starting thread pool\n";
	// declaring the class in the scope forces destructor to run, so then the last print runs after all tasks are done.
	{
		ThreadPool pool(3);
		pool.submit(print, 70, "hello");
		pool.submit([](int a, int b) { std::cout << a * b << "\n"; }, 4, 6);
		//pool.submit()
	}

	std::cout << "Ending thread pool\n";
	return 0;
}
