
#include "./include/thread-pool/thread-pool.h"

using namespace std;

int main()
{
	std::cout << "Starting thread pool\n";
	{
		ThreadPool pool(10);
	}
	std::cout << "Ending thread pool\n";
	return 0;
}
