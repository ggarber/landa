#include <vector>

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

class ThreadPoolThread;
class ThreadPoolExecutor: public Executor {
  public:
  	ThreadPoolExecutor(size_t count);
  	~ThreadPoolExecutor();

  	void Stop();
    void Join();

  	virtual void Schedule(Actor& actor);

  private:
  	size_t count_;
  	std::vector<ThreadPoolThread*> threads_;
};

};
