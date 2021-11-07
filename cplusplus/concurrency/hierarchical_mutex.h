#include <limits.h>
#include <mutex>
#include <stdexcept>

class hierarchical_mutex {
private:
  std::mutex mux;
  const int value;
  int pre;
  static thread_local int this_thread_curr_value;

  void check() {
    if (this_thread_curr_value <= value) {
      throw std::logic_error("hierarchy violated");
    }
  }

  void update() {
    pre = this_thread_curr_value;
    this_thread_curr_value = value;
  }

public:
  explicit hierarchical_mutex(int value) : value(value), pre(0) {}

  void lock() {
    check();
    mux.lock();
    update();
  }

  void unlock() {
    this_thread_curr_value = pre;
    mux.unlock();
  }

  bool try_lock() {
    check();
    if (!mux.try_lock()) {
      return false;
    }
    update();
    return true;
  }
};

thread_local int hierarchical_mutex::this_thread_curr_value(INT_MAX);
