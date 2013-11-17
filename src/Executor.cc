#include "Executor.h"

#include <assert.h>
#include <vector>
#include <thread>
#include <mutex>
#include "Util.h"
#include "Actor.h"

using namespace landa;

using std::thread;

void Executor::Execute(Actor& actor) {
	actor.Execute();
}

void BasicExecutor::Schedule(Actor& actor) {
	Execute(actor);
}

namespace landa {
class ThreadPoolThread {
  public:
	ThreadPoolThread(): thread_(&ThreadPoolThread::Run, this) {
	}

	~ThreadPoolThread() {
		assert(queue_.empty());
	}

	void Stop() {
		Add(nullptr);
	}

	void Join() {
		thread_.join();
	}

	void Run() {
		while (true) {
			if (queue_.empty()) {
    			std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			// TODO: avoid copies with a better queue
			std::vector<Actor*> queue;
			mutex_.lock();
			queue = queue_;
			queue_.clear();
			mutex_.unlock();

			for (auto actor: queue) {
				if (actor == nullptr) {
					return;
				}
				actor->Execute();
				actor->Release();
			}
		}
	}

	void Add(Actor* actor) {
		LANDA_METHOD();

		if (actor != nullptr && !actor->ScheduleIfIdle()) {
			return;
		}
		std::lock_guard<std::mutex> lock(mutex_);
		queue_.push_back(actor);
	}

	std::thread thread_;
	std::mutex mutex_;
	std::vector<Actor*> queue_;
};
};

ThreadPoolExecutor::ThreadPoolExecutor(size_t count): count_(count) {
	for (size_t i = 0; i < count; i++) {
		auto thread = new ThreadPoolThread();
		threads_.push_back(thread);
	}
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
	Stop();
	Join();
}

void ThreadPoolExecutor::Stop() {
	LANDA_METHOD();

	for(auto thread: threads_) {
		thread->Stop();
	}
}

void ThreadPoolExecutor::Join() {
	LANDA_METHOD();

	for(auto thread: threads_) {
		thread->Join();
		delete thread;
	}
	threads_.clear();
}

void ThreadPoolExecutor::Schedule(Actor& actor) {
	assert(!threads_.empty());

	auto thread = threads_[actor.GetHashCode() % count_];

	actor.AddRef();
	thread->Add(&actor);
}
