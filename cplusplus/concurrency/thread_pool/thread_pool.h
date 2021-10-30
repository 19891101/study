#include <atomic>
#include <deque>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

class function_wrapper {
private:
  struct impl_base {
    virtual void call() = 0;
    virtual ~impl_base() {}
  };
  std::unique_ptr<impl_base> impl;

  template <typename F> struct impl_type : impl_base {
    F f;
    explicit impl_type(F &&f) : f(std::move(f)) {}
    void call() override { f(); }
  };

public:
  template <typename F>
  function_wrapper(F &&f) : impl(new impl_type<F>(std::move(f))) {}
  void operator()() { impl->call(); }
  function_wrapper() = default;
  function_wrapper(function_wrapper &&other) : impl(std::move(other.impl)) {}
  function_wrapper &operator=(function_wrapper &&other) {
    impl = std::move(other.impl);
    return *this;
  }
  function_wrapper(const function_wrapper &) = delete;
  function_wrapper(function_wrapper &) = delete;
  function_wrapper &operator=(const function_wrapper &other) = delete;
};

class work_stealing_queue {
private:
  typedef function_wrapper data_type;
  std::deque<data_type> the_queue;
  mutable std::mutex the_mutex;

public:
  work_stealing_queue() = default;
  work_stealing_queue(const work_stealing_queue &) = delete;
  work_stealing_queue &operator=(const work_stealing_queue &) = delete;
  void push(data_type data) {
    std::lock_guard<std::mutex> g(the_mutex);
    the_queue.push_front(std::move(data));
  }
  bool empty() const {
    std::lock_guard<std::mutex> g(the_mutex);
    return the_queue.empty();
  }
  bool try_pop(data_type *result) {
    std::lock_guard<std::mutex> g(the_mutex);
    if (the_queue.empty()) {
      return false;
    }
    *result = std::move(the_queue.front());
    the_queue.pop_front();
    return true;
  }
  bool try_steal(data_type *result) {
    std::lock_guard<std::mutex> g(the_mutex);
    if (the_queue.empty()) {
      return false;
    }
    *result = std::move(the_queue.back());
    the_queue.pop_back();
    return true;
  }
};

template <typename T> class thread_safe_queue {
private:
  std::queue<T> que;
  mutable std::mutex mux;

public:
  void push(T d) {
    std::lock_guard<std::mutex> g(mux);
    que.push(std::move(d));
  }
  bool try_pop(T *result) {
    std::lock_guard<std::mutex> g(mux);
    if (que.empty()) {
      return false;
    }
    *result = std::move(que.front());
    que.pop();
    return true;
  }
};

class thread_pool {
private:
  typedef function_wrapper task_type;
  std::atomic_bool done;
  thread_safe_queue<task_type> pool_work_queue;
  std::vector<std::unique_ptr<work_stealing_queue>> queues;
  std::vector<std::thread> threads;

  static thread_local work_stealing_queue *local_work_queue;
  static thread_local size_t my_index;

  bool pop_task_from_local_queue(task_type *task) {
    return local_work_queue && local_work_queue->try_pop(task);
  }
  bool pop_task_from_pool_queue(task_type *task) {
    return pool_work_queue.try_pop(task);
  }
  bool pop_task_from_other_thread_queue(task_type *task) {
    for (size_t i = 0; i < queues.size(); ++i) {
      size_t id = (my_index + i + 1) % queues.size();
      if (queues[id]->try_pop(task)) {
        return true;
      }
    }
    return false;
  }

public:
  explicit thread_pool(size_t thread_num) : done(false) {
    for (size_t i = 0; i < thread_num; ++i) {
      queues.push_back(
          std::unique_ptr<work_stealing_queue>(new work_stealing_queue()));
    }
    for (size_t i = 0; i < thread_num; ++i) {
      threads.push_back(std::thread(&thread_pool::worker_thread, this, i));
    }
  }
  ~thread_pool() {
    done = true;
    for (auto &th : threads) {
      if (th.joinable()) {
        th.join();
      }
    }
  }
  void worker_thread(size_t index) {
    my_index = index;
    local_work_queue = queues[index].get();
    while (!done) {
      run_pending_task();
    }
  }

  template <typename FunctionType>
  std::future<typename std::result_of<FunctionType()>::type>
  submit(FunctionType f) {
    typedef typename std::result_of<FunctionType()>::type result_type;
    std::packaged_task<result_type()> task(f);
    std::future<result_type> result(task.get_future());
    if (local_work_queue) {
      local_work_queue->push(std::move(task));
    } else {
      pool_work_queue.push(std::move(task));
    }
    return result;
  }
  void run_pending_task() {
    task_type task;
    if (pop_task_from_local_queue(&task) || pop_task_from_pool_queue(&task) ||
        pop_task_from_other_thread_queue(&task)) {
      task();
    } else {
      std::this_thread::yield();
    }
  }
};

thread_local work_stealing_queue *thread_pool::local_work_queue;
thread_local size_t thread_pool::my_index;
