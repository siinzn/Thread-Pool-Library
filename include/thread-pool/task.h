

/*
First, write the abstract base class + one templated derived class, completely standalone, in a scratch file — no thread pool involved. 
Prove to yourself you can create a TaskImpl<SomeLambdaType> on the heap, store it as unique_ptr<TaskBase>, and call execute() through the base pointer, 
and watch the right lambda run. This isolates the "type erasure" idea from all your existing thread pool complexity.

Once that's solid, swap your queue's type from std::queue<std::function<void()>> to std::queue<std::unique_ptr<TaskBase>>.

Update submit() to build a TaskImpl instead of the shared_ptr version.

Update payload() to move (not copy) out of the queue, and call ->execute() through the base pointer.

Rerun ALL your existing main.cpp tests (regular functions, lambdas, mutable counter, future+get, exception+get) — every single one should still behave identically to before. 
If they do, Phase 5 is done, and you've removed the shared_ptr overhead entirely.
*/

#include <iostream>

int task();

class TaskBase {
public:
	virtual ~TaskBase() = default; // can also be written as virtual ~TaskBase() {}
	virtual void execute() = 0;	
};

template<typename F>
struct TaskImpl : TaskBase { 
	/*
	here it is a struct because struct demands public inheritance, and class is by default private inheritance then
	taskbase cannot be accessed outside of the class, but with struct its fine or i have to do class TaskImpl : public TaskBase which would let me call it from outside
	*/
public:
	TaskImpl(F value_) : value(value_) {};
	void execute() override {
		value();
	}
private:
	F value;
};

template <typename F> TaskImpl(F) -> TaskImpl<F>;

