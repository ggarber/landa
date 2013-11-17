# landa

C++ concurrency library based on Actor pattern and C++11 features.

The library offers a framework to create entities (Actors) that can potentially run in parallel and send and receive messages to each other without concurrency issues.

The Actor pattern is available in other languages like Erlang or Scala and simplifies the development of highly concurrent applications:
http://www.slideshare.net/drorbr/the-actor-model-towards-better-concurrency

There are other actor model frameworks for C++ but it is always fun to create your own implementation and then compare it with other solutions:
http://en.wikipedia.org/wiki/Actor_model#Actor_libraries_and_frameworks

Instead of using messages and pattern matching like in other languages, the messages in landa are callable objects (std::function, lambda, function object).   It gives probably too much flexibility allowing requesting an actor to execute code in other actor but it is a very simple solution and fits great with C++11 lambdas/closures.

The library is under development but already usable with an initial implementation of executor distributing load by sharding actors.   This is an example of usage with two actors running in parallel and receiving a message (one from the main application, and the other from the other actor):

```
#include "landa/Actor.h"

using landa::Actor;

class MyActor1: Actor {
  public:
    MyActor1(Executor& executor): Actor(executor) {}
    
    void Action1() { 
        std::cout << "Action1 executed" << std::endl;
    }
};

public class MyActor2: Actor {
  public:
    MyActor2(Executor& executor): Actor(executor) {}
    
    void Action2(MyActor1* actor) {
        std::cout << "Action2 executed" << std::endl;
        
        *actor << std::bind(&MyActor1::Action1, actor);
    }
};


int main() {
    ThreadPoolExecutor executor(10);  // Number of threads
    
    Actor* actor1 = new MyActor1(executor);
    Actor* actor2 = new MyActor2(executor);
    
    *actor2 << std::bind(&MyActor2::Action2, actor2, actor1);
    
    actor1->Release();
    actor2->Release();
}

```

# Build

* clone the repo
* mkdir build; cd build; cmake ..; make
* ./LandaTest

# Next Steps

* Evaluate other executors based on shared queue between threads (initial tests shows it is slower at least for some scenarios).
* Add unit tests and documentation
* Fool somebody to use it in any real project

