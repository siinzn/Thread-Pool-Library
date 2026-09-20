// this is a test file to check how Taskbase & TaskImpl works

#include "../include/thread-pool/task.h"
#include <memory>

int task() {
	auto lambda1 = []() { std::cout << "This is inside lambda type\n"; };
	using LambdaType = decltype(lambda1);
	std::unique_ptr<TaskBase> ptr = std::make_unique<TaskImpl<LambdaType>>(std::move(lambda1));
	ptr->execute();
	return 0;
}