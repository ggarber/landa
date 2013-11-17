#include <iostream>
#include "Actor.h"
#include "Executor.h"

using landa::Actor;
using landa::Executor;
using landa::BasicExecutor;

class Actor1: public Actor {
  public:
  	Actor1(Executor& executor): Actor(executor) {};
};

class Actor2: public Actor {
  public:
  	Actor2(Executor& executor): Actor(executor) {};
};

int main() {
    BasicExecutor executor;

    Actor1 actor1(executor);
    actor1 << [] () { std::cout << "MSG1" << std::endl; };
}