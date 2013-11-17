#include "Executor.h"
#include "Actor.h"

using namespace landa;

using std::thread;

void Executor::Execute(Actor& actor) {
	actor.Execute();
}

void BasicExecutor::Schedule(Actor& actor) {
	Execute(actor);
}

ThreadPoolExecutor::ThreadPoolExecutor(size_t count) {
	threads_.push_back(thread(ThreadPoolExecutor::Run));
}

void ThreadPoolExecutor::Schedule(Actor& actor) {
	Execute(actor);
}

void ThreadPoolExecutor::Run() {
}
