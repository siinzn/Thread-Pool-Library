
#include "./include/thread-pool/thread-pool.h"

using namespace std;

int main()
{
	std::cout << "Starting thread pool\n";
	ThreadPool pool(10);

	for (int i = 0; i < 8; ++i) {
		pool.submit([i]() { std::cout << "Executing task " << i << "\n"; });
	}

	std::cout << "Ending thread pool\n";
	return 0;
}
