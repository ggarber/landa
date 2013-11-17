#include <functional>
#include <mutex>
#include <vector>

namespace landa {

class Executor;

class Actor {
  public:
  	typedef std::function<void ()> Task;

  public:
  	Actor(landa::Executor& executor);
  	virtual ~Actor();

  	Actor& operator <<(Task func);

  private:
  	void Execute();

  private:
  	landa::Executor& executor_;
  	std::mutex tasks_mutex_;
  	std::vector<Task> tasks_;

  	friend class landa::Executor;
};

};
