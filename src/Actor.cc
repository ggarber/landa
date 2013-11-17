#include "Actor.h"

#include <assert.h>
#include <random>
#include "Executor.h"

using namespace landa;

//TODO: Not thread safe
static std::mt19937 rng;
static std::uniform_int_distribution<size_t> hash_dist;

Actor::Actor(Executor& executor): ref_(1), hash_code_(hash_dist(rng)), scheduled_(false), executor_(executor) {
}

Actor::~Actor() {
	assert(ref_ == 0);
	assert(tasks_.empty());
}

Actor& Actor::operator <<(Actor::Task task) {
	Add(task);

	executor_.Schedule(*this);
	return *this;
}

void Actor::Add(Actor::Task task) {
	std::lock_guard<std::mutex> lock(tasks_mutex_);
	tasks_.push_back(task);
}

void Actor::Execute() {
	//TODO: Do not call it inside a lock
	std::lock_guard<std::mutex> lock(tasks_mutex_);
	for (auto task: tasks_) {
		task();
	}
	tasks_.clear();

	scheduled_ = false;
}
