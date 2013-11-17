#include <assert.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "Actor.h"
#include "Executor.h"

using landa::Actor;
using landa::Executor;
using landa::BasicExecutor;
using landa::ThreadPoolExecutor;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

class Actor1: public Actor {
  public:
  	Actor1(Executor& executor): Actor(executor), executed_(0) {};

  	void Shot1(const std::string& msg) {
  		std::cout << msg << std::endl;
  		executed_++;
  	}

  	void Shot2() {
  		int a = 0;
  		for (size_t i = 0; i < 10000; i++) {
  			a = i*i;
  		};
  		executed_++;
  	}

  	void Shot3() {
  		executed_++;
  	}

  	uint32_t executed_;
};

class Actor2: public Actor {
  public:
  	Actor2(Executor& executor): Actor(executor) {};
};

class Action {
  public:
  	void operator() () { std::cout << "MSG_3" << std::endl; }
};

void basic_test() {
    ThreadPoolExecutor executor(10);

    Actor1* actor1 = new Actor1(executor);
    *actor1 << [] () { std::cout << "MSG_1" << std::endl; };
    *actor1 << bind(&Actor1::Shot1, actor1, "MSG_2");
    *actor1 << Action();

    executor.Stop();
    executor.Join();

    assert(actor1->executed_ == 1);
    actor1->Release();
}

void communication_test() {
    ThreadPoolExecutor executor(10);

    Actor1* actor1 = new Actor1(executor);
    Actor1* actor2 = new Actor1(executor);

    *actor1 << [actor2] () { *actor2 << std::bind(&Actor1::Shot3, actor2); };
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    executor.Stop();
    executor.Join();

    assert(actor2->executed_ == 1);
    actor1->Release();
    actor2->Release();
}

void timer_test() {
    /*ThreadPoolExecutor executor(10);

    Actor1 actor1(executor);
    actor1 << landa::delay(10)
           << [] () { std::cout << "MSG1" << std::endl; };
    actor1 << landa::repeat("aa", 10)
           << [] () { std::cout << "MSG1" << std::endl; };

    executor.Stop();
    executor.Join();*/
}

static const uint32_t kActors = 1000;
static const uint32_t kItems = 100;

void perf_test() {
	auto start = high_resolution_clock::now();

    ThreadPoolExecutor executor(1);

    std::vector<Actor1*> actors_;
    for (size_t i = 0; i < kActors; i++) {
    	actors_.push_back(new Actor1(executor));
    }

    for (size_t i = 0; i < kActors * kItems; i++) {
    	auto actor = actors_[i % actors_.size()];
    	*actor << [actor] () { actor->Shot2(); };
	}

    executor.Stop();
    executor.Join();

    for (auto actor: actors_) {
    	assert(actor->executed_ == kItems);
    	actor->Release();
    }
	auto end = high_resolution_clock::now();

	std::cout << "Time: " <<  duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;
}

int main() {
	basic_test();
	communication_test();
	timer_test();
	perf_test();
}