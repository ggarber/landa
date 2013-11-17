#include <functional>
#include <vector>
#include <mutex>
#include <atomic>

namespace landa {

class Executor;

class Actor {
  public:
  	typedef std::function<void ()> Task;

  public:
  	Actor(landa::Executor& executor);
  	virtual ~Actor();

  	Actor& operator <<(Task task);

  	void Execute();

    inline void AddRef() { ref_++; }
    inline void Release() { if (--ref_ == 0) { delete this; } }
  	inline size_t GetHashCode() const { return hash_code_; }
  	inline bool ScheduleIfIdle() {
  		std::lock_guard<std::mutex> lock(tasks_mutex_);
  		bool tst = false;
  		return scheduled_.compare_exchange_strong(tst, true);
  	}

  private:
  	void Add(Task task);

  private:
  	std::atomic<uint32_t> ref_;
  	size_t hash_code_;
  	std::atomic<bool> scheduled_; 

  	landa::Executor& executor_;
  	std::mutex tasks_mutex_;
  	std::vector<Task> tasks_;

  	friend class landa::Executor;
};

};
