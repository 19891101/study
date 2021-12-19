#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "lock_free_stack.h"
#include "lock_free_stack_v2.h"

// lock_free_stack<int> st;
LockFreeStackV2<int> st;

std::atomic<bool> go(false);
std::atomic<bool> stop(false);
std::atomic<int> pop_num;
std::atomic<int> index;

std::vector<std::pair<int, std::pair<int, int>>> logs;

void add(int a, int b, int c) {
  int i = index++;
  logs[i].first = a;
  logs[i].second.first = b;
  logs[i].second.second = c;
}

void push(const std::vector<int> &eles, int id) {
  while (!go) {
    std::this_thread::yield();
  }
  for (int x : eles) {
    add(id, 1, x);
    st.push(x);
    std::this_thread::yield();
  }
}

void pop(int id) {
  while (!go) {
    std::this_thread::yield();
  }
  while (!stop) {
    auto p = st.pop();
    if (p != nullptr) {
      add(id, 2, *p);
      ++pop_num;
    }
  }
}

int main() {

  std::vector<int> s = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  logs.resize(36);
  std::thread t1(push, s, 1);
  std::thread t2(push, s, 2);
  std::thread t3(pop, 1);
  std::thread t4(pop, 2);
  std::thread t5(pop, 3);
  std::thread t6(pop, 4);
  go = true;
  while (pop_num != 18) {
    std::this_thread::yield();
  }
  stop = true;
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  for (const auto &s : logs) {
    if (s.second.first == 1) {
      std::cout << "push " << s.first << " " << s.second.second << "\n";
    } else {
      std::cout << "pop ---->>> " << s.first << " " << s.second.second << "\n";
    }
  }
  return 0;
}
