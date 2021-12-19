#include <atomic>
#include <functional>
#include <memory>
#include <stdexcept>
#include <thread>

constexpr int kMaxHazardPointers = 128;

struct HazardPointer {
  std::atomic<std::thread::id> id;
  std::atomic<void *> pointer;
};

HazardPointer hazard_pointers[kMaxHazardPointers];

class HazardPointerOwner {
public:
  HazardPointerOwner(const HazardPointerOwner &) = delete;
  HazardPointerOwner &operator=(const HazardPointerOwner &) = delete;
  HazardPointerOwner() : hp(nullptr) {
    for (int i = 0; i < kMaxHazardPointers; ++i) {
      std::thread::id old_id;
      if (hazard_pointers[i].id.compare_exchange_strong(
              old_id, std::this_thread::get_id())) {
        hp = hazard_pointers + i;
        return;
      }
    }
    throw std::runtime_error("No hazard pointers available");
  }
  ~HazardPointerOwner() {
    hp->pointer.store(nullptr);
    hp->id.store(std::thread::id());
  }

  std::atomic<void *> *get_pointer() { return &(hp->pointer); }

private:
  HazardPointer *hp;
};

std::atomic<void *> *GetCurrentThreadHazardPointer() {
  thread_local static HazardPointerOwner hazard;
  return hazard.get_pointer();
}

bool NoOtherThreadAccess(void *p) {
  for (int i = 0; i < kMaxHazardPointers; ++i) {
    if (hazard_pointers[i].pointer.load() == p) {
      return false;
    }
  }
  return true;
}

template <typename T> void DoDeleter(void *p) { delete static_cast<T *>(p); }

struct DataToReclaim {
  void *data;
  std::function<void(void *)> deleter;
  DataToReclaim *next;

  template <typename T>
  DataToReclaim(T *p) : data(p), deleter(&DoDeleter<T>), next(nullptr) {}

  ~DataToReclaim() { deleter(data); }
};

std::atomic<DataToReclaim *> node_to_reclaim;

void AddToReclaimList(DataToReclaim *node) {
  node->next = node_to_reclaim.load();
  while (!node_to_reclaim.compare_exchange_weak(node->next, node))
    ;
}

template <typename T> void ReclaimLater(T *data) {
  AddToReclaimList(new DataToReclaim(data));
}

void DeleteNodesWithNoHazards() {
  DataToReclaim *current = node_to_reclaim.exchange(nullptr);
  while (current != nullptr) {
    DataToReclaim *next = current->next;
    if (NoOtherThreadAccess(current->data)) {
      delete current;
    } else {
      AddToReclaimList(current);
    }
    current = next;
  }
}

template <typename T> class LockFreeStackV2 {
public:
  void push(const T &data) {
    node *new_node = new node(data);
    new_node->next = head_.load();
    while (!head_.compare_exchange_weak(new_node->next, new_node))
      ;
  }

  std::shared_ptr<T> pop() {
    std::atomic<void *> *hp = GetCurrentThreadHazardPointer();
    node *old_head = head_.load();
    do {
      node *temp;
      do {
        temp = old_head;
        hp->store(old_head);
        old_head = head_.load();
      } while (old_head != temp);
    } while (old_head &&
             !head_.compare_exchange_strong(old_head, old_head->next));
    hp->store(nullptr);
    std::shared_ptr<T> result;
    if (old_head != nullptr) {
      result.swap(old_head->data);
      if (NoOtherThreadAccess(old_head)) {
        delete old_head;
      } else {
        ReclaimLater(old_head);
      }
      DeleteNodesWithNoHazards();
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
};
