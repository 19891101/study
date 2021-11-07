#include <shared_mutex>

class RWMutex {
public:
  std::shared_lock<std::shared_mutex> reader() {
    return std::shared_lock<std::shared_mutex>(mutex_);
  }

  std::unique_lock<std::shared_mutex> writer() {
    return std::unique_lock<std::shared_mutex>(mutex_);
  }

private:
  mutable std::shared_mutex mutex_;
};

