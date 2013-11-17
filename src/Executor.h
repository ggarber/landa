#include <vector>
#include <thread>

namespace landa {

class Actor;

class Executor {
  public:
  	virtual ~Executor() = default;
  	virtual void Schedule(Actor& actor) = 0;

  protected:
  	void Execute(Actor& actor);
};

class BasicExecutor: public Executor {
  public:
  	virtual void Schedule(Actor& actor);
};

class ThreadPoolExecutor: public Executor {
  public:
  	ThreadPoolExecutor(size_t count);
  	virtual void Schedule(Actor& actor);

  private:
  	static void Run();

  private:
  	std::vector<std::thread> threads_;
};

};
