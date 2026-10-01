
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

		std::cout << "\n[TEST 1] Function with return value:\n";
		std::future<int> add_return = pool.submit([](int a, int b) {return a + b;}, 10, 5);
		std::future<int> multi_return = pool.submit([](int a, int b) {return a * b;}, 10, 5);
		std::cout << "Sum : " << add_return.get() << "\n";
		std::cout << "Product: " << multi_return.get() << "\n";

		std::cout << "\n[TEST 2] Exception handling:\n";
		std::future<int> excep = pool.submit([]() {
			throw std::runtime_error("run time error");
			return 21;
			});
		try {
			std::cout << "Error: " << excep.get() << "\n";
		}
		catch (const std::exception& e) {
			std::cout << "Caught from future: " << e.what() << "\n";
		}

		std::cout << "\n[TEST 3] Submit 50 tasks (more than thread count):\n";
		int counter = 0;
		int tasks = 5000;
		std::mutex counter_mutex;

		std::vector<std::future<void>> futures;
		for (int i{ 0 }; i < tasks; i++) {
			std::future<void> f = pool.submit([&counter, &counter_mutex]() {
				std::lock_guard<std::mutex> lock(counter_mutex);
				counter++;
				});
			futures.push_back(std::move(f));
		}
		
		for (auto& f : futures) {
			f.get();
		}
		std::cout << "Submitted: " << tasks << ", Completed: " << counter << "\n";


		std::cout << "\n[TEST 4] Task capturing std::unique_ptr:\n";
		auto ptr = std::make_unique<int>(42);
		pool.submit([ptr = std::move(ptr)]() {
			std::cout << "unique_ptr value = " << *ptr << "\n";
			});

		std::cout << "\n[TEST 5] Pool shutdown waits for pending tasks:\n";
		pool.submit([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::cout << "Task 1 executing...\n";
			});

		pool.submit([]() {
			std::cout << "Task 2 executing...\n";
			});

	}
	std::cout << "Ending thread pool\n";
	return 0;
}
