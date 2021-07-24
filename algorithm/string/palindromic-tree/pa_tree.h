#pragma once

#include <array>
#include <vector>

namespace alg {
namespace string {

template <int N, typename Self>
class PalindromicTree {
  static constexpr size_t default_next = 0;
  static constexpr size_t empty_node = 0;

  struct Node {
    std::array<size_t, N> sons;
    size_t fail;
    int len;
    Self self;
  };

 public:
  explicit PalindromicTree(size_t size = 0) {
    pools_.reserve(size + 2);
    contents_.reserve(size + 2);
    size_t even_root = NewNode(0);
    size_t odd_root = NewNode(-1);
    pools_[even_root].fail = odd_root;
    last_ = even_root;
    contents_.emplace_back(-1, even_root);
  }

  class PalindromicTreeNode {
   public:
    int length() const {
      return pools[index].len;
    }
    Self &self() {
      return pools[index].self;
    }
    PalindromicTreeNode son(int id) const {
      return PalindromicTreeNode(pools, pools[index].sons[id]);
    }
    PalindromicTreeNode fail() const {
      return PalindromicTreeNode(pools, pools[index].fail);
    }
    size_t hash_index() const {
      return index;
    }
    bool even_root() const {
      return index == 0;
    }
    bool odd_root() const {
      return index == 1;
    }

    PalindromicTreeNode(std::vector<Node> &pools, size_t index)
        : pools(pools), index(index) {}

   private:
    std::vector<Node> &pools;
    size_t index;
  };

  // The added 'c' should be in [0, N-1]
  bool Add(int c) {
    contents_.emplace_back(c, 0);
    size_t curr = GetFail(last_);
    bool new_added = false;
    if (pools_[curr].sons[c] == empty_node) {
      size_t new_node = NewNode(pools_[curr].len + 2);
      pools_[new_node].fail = pools_[GetFail(pools_[curr].fail)].sons[c];
      pools_[curr].sons[c] = new_node;
      new_added = true;
    }
    last_ = pools_[curr].sons[c];
    contents_.back().second = last_;
    return new_added;
  }

  // Get the tree node that corresponding with the longest palindromic end with
  // the index 'id'm which is the id-th called 'Add'
  PalindromicTreeNode GetNodeByInsertIndex(size_t id) {
    return PalindromicTreeNode(pools_, contents_[id].second);
  }

  size_t NodeNumber() const {
    return pools_.size() - 2;
  }

  PalindromicTreeNode GetNodeByIndex(size_t id) {
    return PalindromicTreeNode(pools_, id + 2);
  }

  PalindromicTreeNode OddRoot() {
    return PalindromicTreeNode(pools_, 1);
  }

  PalindromicTreeNode EvenRoot() {
    return PalindromicTreeNode(pools_, 0);
  }

 private:
  size_t NewNode(int len) {
    pools_.emplace_back();
    size_t last = pools_.size() - 1;
    for (int i = 0; i < N; ++i) {
      pools_[last].sons[i] = default_next;
    }
    pools_[last].len = len;
    return last;
  }

  size_t GetFail(size_t curr) {
    size_t n = contents_.size() - 1;
    int last_ch = contents_.back().first;
    while (contents_[n - pools_[curr].len - 1].first != last_ch) {
      curr = pools_[curr].fail;
    }
    return curr;
  }

  std::vector<Node> pools_;
  std::vector<std::pair<int, size_t>> contents_;
  size_t last_;
};

}  // namespace string
}  // namespace alg
