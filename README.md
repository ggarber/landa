landa
=====

C++ concurrency library based on Actor pattern and C++11 features.

The library offers a framework to create entities (Actors) that can potentially run in parallel and send and receive messages to each other without concurrency issues.

The Actor pattern is available in other languages like Erlang or Scala and simplifies the development of highly concurrent applications:
http://www.slideshare.net/drorbr/the-actor-model-towards-better-concurrency

Instead of using messages and pattern matching like in other languages, the messages in landa are callable objects.

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

Use any callable object (lambda function, object redefining operator()...) for messaging passing.

