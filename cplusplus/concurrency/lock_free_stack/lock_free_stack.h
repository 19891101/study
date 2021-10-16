#include <atomic>
#include <memory>

template <typename T> class lock_free_stack {
public:
  void push(const T &data) {
    node *new_node = new node(data);
    new_node->next = head_.load();
    while (!head_.compare_exchange_weak(new_node->next, new_node))
      ;
  }
  std::shared_ptr<T> pop() {
    ++thread_in_loop_;
    node *old_head = head_.load();
    while (old_head != nullptr &&
           !head_.compare_exchange_weak(old_head, old_head->next))
      ;
    std::shared_ptr<T> result;
    if (old_head != nullptr) {
      result.swap(old_head->data);
      try_reclaim(old_head);
    }

    return result;
  }

private:
  struct node {
    std::shared_ptr<T> data;
    node *next;

    node(const T &d) : data(std::make_shared<T>(d)), next(nullptr) {}
  };
  std::atomic<node *> head_;
  std::atomic<unsigned int> thread_in_loop_;
  std::atomic<node *> to_be_deleted_;

  void delete_nodes(node *nodes) {
    while (nodes != nullptr) {
      node *next = nodes->next;
      delete nodes;
      nodes = next;
    }
  }
  void chanin_pending_nodes(node *nodes) {
    node *last = nodes;
    for (node *next = last->next; next != nullptr; next = last->next) {
      last = next;
    }
    chanin_pending_nodes(nodes, last);
  }
  void chanin_pending_nodes(node *first, node *last) {
    last->next = to_be_deleted_;
    while (!to_be_deleted_.compare_exchange_weak(last->next, first))
      ;
  }
  void try_reclaim(node *old_head) {
    if (thread_in_loop_ == 1) {
      node *nodes_to_delete = to_be_deleted_.exchange(nullptr);
      if (--thread_in_loop_ == 0) {
        delete_nodes(nodes_to_delete);
      } else if (nodes_to_delete != nullptr) {
        chanin_pending_nodes(nodes_to_delete);
      }
      delete old_head;
    } else {
      chanin_pending_nodes(old_head, old_head);
      --thread_in_loop_;
    }
  }
};

