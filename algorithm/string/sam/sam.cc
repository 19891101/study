
#include <unordered_map>
#include <vector>

class SAM {
public:
  SAM(int size) : pools(size * 2), use_index(1), last_index(0) {
    pools[0].length = 0;
    pools[0].link = -1;
  }

  void Add(int c) {
    int curr_index = use_index++;
    Node &curr = pools[curr_index];
    Node &last = pools[last_index];
    curr.length = last.length + 1;
    int p = last_index;
    while (p != -1 && 0 == pools[p].next.count(c)) {
      pools[p].next[c] = curr_index;
      p = pools[p].link;
    }
    if (p == -1) {
      curr.link = 0;
    } else {
      int q = pools[p].next[c];
      if (pools[p].length + 1 == pools[q].length) {
        curr.link = q;
      } else {
        int clone_index = use_index++;
        Node &clone = pools[clone_index];
        clone.length = pools[p].length + 1;
        clone.next = pools[q].next;
        clone.link = pools[q].link;
        for (; p != -1 && pools[p].next[c] == q; p = pools[p].link) {
          pools[p].next[c] = clone_index;
        }
        pools[q].link = curr.link = clone_index;
      }
    }
    last_index = curr_index;
  }

private:
  struct Node {
    int length;
    int link;
    std::unordered_map<int, int> next;
  };
  std::vector<Node> pools;
  int use_index;
  int last_index;
};

int main() { return 0; }
