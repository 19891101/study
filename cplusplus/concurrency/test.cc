#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "hierarchical_mutex.h"
#include "rw_mutex.h"
#include "threadsafe_queue.h"

void test_hierarchical_mutex() {
  hierarchical_mutex a(100);
  hierarchical_mutex b(50);
  std::lock_guard<hierarchical_mutex> g1(a);
  std::lock_guard<hierarchical_mutex> g2(b);
}

void test_rw_mutex() {
  int x = 0;
  RWMutex mutex;
  std::atomic<bool> start = false;
  auto add = [&](int delta) {
    while (!start) {
      std::this_thread::yield();
    }
    for (int i = 0; i < 10; ++i) {
      auto g = mutex.writer();
      x += delta;
      std::this_thread::yield();
    }
  };
  auto get = [&](int id) {
    while (!start) {
      std::this_thread::yield();
    }
    for (int i = 0; i < 5; ++i) {
      auto g = mutex.reader();
      std::cout << id << ": " << x << std::endl;
      std::this_thread::yield();
    }
  };
  std::vector<std::thread> all;
  for (int i = 0; i < 5; ++i) {
    all.push_back(std::thread(get, i));
  }
  for (int i = 0; i < 3; ++i) {
    all.push_back(std::thread(add, (i + 1) * 5));
  }
  start = true;
  for (auto &e : all) {
    if (e.joinable()) {
      e.join();
    }
  }
  std::cout << "last: " << x << std::endl;
}

void test_threadsafe_queue() {
  struct node {
    int x, y;
  };
  threadsafe_queue<node> que;
  auto pop = [&](int id) {
    while (true) {
      auto d = que.wait_and_pop();
      std::cout << id << ": " << d->x << ", " << d->y << std::endl;
    }
  };
  auto push = [&]() {
    int x, y, t;
    std::cout << "input: ";
    while (std::cin >> x >> y >> t) {
      for (int i = 0; i < t; ++i) {
        node p;
        p.x = x;
        p.y = y;
        que.push(p);
      }
      std::cout << "input: ";
    }
  };
  std::vector<std::thread> all;
  for (int i = 0; i < 4; ++i) {
    all.emplace_back(pop, i);
  }
  all.emplace_back(push);
  for (auto &e : all) {
    if (e.joinable()) {
      e.join();
    }
  }
}

int main() {
  //  test_rw_mutex();
  test_threadsafe_queue();
  return 0;
}
