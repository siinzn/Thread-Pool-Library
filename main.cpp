
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

		// test to see if different types of params work
		pool.submit(print, 70, "hello");
		// test for lambda
		pool.submit([](int a, int b) { std::cout << a * b << "\n"; }, 4, 6);
		// test to see if mutable works
		int counter = 0;
		pool.submit([counter]() mutable {
			counter++;
			std::cout << "Counter: " << counter << "\n";
			});
		// future get test
		std::future<int> add = pool.submit([](int a, int b) {return a + b;}, 5,7);
		std::cout << "Result: " << add.get() << "\n";

		//exception
		std::future<int> excep = pool.submit([]() {
			throw std::runtime_error("Blablabla");
			return 21;
		});
		try {
			std::cout << "Error: " << excep.get() << "\n";
		}
		catch (const std::exception& e) {
			std::cout << "Caught from future: " << e.what() << "\n";
		}
	}

	std::cout << "Ending thread pool\n";

	task();
	return 0;
}
