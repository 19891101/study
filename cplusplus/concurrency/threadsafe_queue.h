#include <condition_variable>
#include <memory>
#include <mutex>

template <typename T> class threadsafe_queue {
private:
  struct node {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
  };
  mutable std::mutex head_mutex;
  std::unique_ptr<node> head;
  mutable std::mutex tail_mutex;
  node *tail;
  std::condition_variable cv;

  node *get_tail() {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }
  std::unique_ptr<node> pop_head() {
    std::unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }
  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    cv.wait(head_lock, [&]() { return head.get() != get_tail(); });
    return std::move(head_lock);
  }

public:
  threadsafe_queue() : head(new node), tail(head.get()) {}

  threadsafe_queue(const threadsafe_queue &) = delete;
  threadsafe_queue &operator=(const threadsafe_queue &) = delete;

  void push(T new_value) {
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<node> p(new node);
    {
      std::lock_guard<std::mutex> tail_lock(tail_mutex);
      tail->data = new_data;
      node *new_tail = p.get();
      tail->next = std::move(p);
      tail = new_tail;
    }
    cv.notify_one();
  }
  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head()->data;
  }
  void wait_and_pop(T *value) {
    std::unique_ptr<node> old_head;
    {
      std::unique_lock<std::mutex> head_lock(wait_for_data());
      old_head = pop_head();
    }
    *value = std::move(*(old_head->data));
  }
  std::shared_ptr<T> try_pop() {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return nullptr;
    }
    return pop_head()->data;
  }
  bool try_pop(T *value) {
    std::unique_ptr<node> old_head;
    {
      std::unique_lock<std::mutex> head_lock(head_mutex);
      if (head.get() == get_tail()) {
        return false;
      }
      old_head = pop_head();
    }
    *value = std::move(*(old_head->data));
    return true;
  }
  bool empty() const {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    return head.get() == get_tail();
  }
};
