#include <atomic>
#include <memory>

template <typename T> class LockFreeStackV3 {
public:
  ~LockFreeStackV3() {
    while (pop())
      ;
  }

  void push(const T &data) {
    NodePtr new_node;
    new_node.ptr = new Node(data);
    new_node.external_count = 1;
    new_node.ptr->next = head.load();
    while (!head.compare_exchange_weak(new_node.ptr->next, new_node))
      ;
  }

  std::shared_ptr<T> pop() {
    NodePtr old_head = head.load();
    while (true) {
      IncreaseHeadCount(old_head);
      Node *ptr = old_head.ptr;
      if (ptr == nullptr) {
        return nullptr;
      }
      if (head.compare_exchange_strong(old_head, ptr->next)) {
        std::shared_ptr<T> result;
        result.swap(ptr->data);
        int delta = old_head.external_count - 2;
        if (ptr->internal_count.fetch_add(delta) == -delta) {
          delete ptr;
        }
        return result;
      } else if (ptr->internal_count.fetch_sub(1) == 1) {
        delete ptr;
      }
    }
  }

private:
  struct Node;
  struct NodePtr {
    int external_count;
    Node *ptr;
  };
  struct Node {
    std::shared_ptr<T> data;
    std::atomic<int> internal_count;
    NodePtr next;

    explicit Node(const T &data)
        : data(std::make_shared<T>(data)), internal_count(0) {}
  };
  std::atomic<NodePtr> head;

  void IncreaseHeadCount(NodePtr &old_counter) {
    NodePtr new_counter;
    do {
      new_counter = old_counter;
      ++new_counter.external_count;
    } while (!head.compare_exchange_strong(old_counter, new_counter));
    old_counter.external_count = new_counter.external_count;
  }
};
