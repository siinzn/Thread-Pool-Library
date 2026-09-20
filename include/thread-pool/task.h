
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
	TaskImpl(F&& value_) : value(std::forward<F>(value_)) {};
	void execute() override {
		value();
	}
private:
	F value;
};

//template <typename F> TaskImpl(F) -> TaskImpl<F>; this is not needed but this is a user guided deduction

