
#include "./include/thread-pool/thread-pool.h"

using namespace std;

int main()
{
	std::cout << "Starting thread pool\n";
    // declaring the class in the scope forces destructor to run, so then the last print runs after all tasks are done.
    {
        ThreadPool pool(3);
        pool.submit([]() {
            std::cout << "Task 1 is running normally.\n";
            });

        pool.submit([]() {
            std::cout << "Task 2 is throwing an exception...\n";
            throw std::runtime_error("Something went wrong inside the task!");
            });

        pool.submit([]() {
            std::cout << "Task 3 is running normally.\n";
            });
    }

	std::cout << "Ending thread pool\n";
	return 0;
}
