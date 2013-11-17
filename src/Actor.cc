#include "Actor.h"

#include "Executor.h"

using namespace landa;

Actor::Actor(Executor& executor): executor_(executor) {
}

Actor::~Actor() {
}

Actor& Actor::operator << (Actor::Task task) {
	// TODO: disable for testing std::lock_guard<std::mutex> lock(tasks_mutex_);
	tasks_.push_back(task);

	executor_.Schedule(*this);
	return *this;
}

void Actor::Execute() {
	std::lock_guard<std::mutex> lock(tasks_mutex_);
	for (auto task: tasks_) {
		task();
	}
}
