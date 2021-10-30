#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "thread_pool.h"

thread_pool pool(10);

void print() { std::cout << "print()" << std::endl; }
int print2(int x) {
  std::cout << "print3()" << std::endl;
  return x * 3;
}
int print1() {
  std::cout << "print1()" << std::endl;
  auto f = pool.submit(std::bind(&print2, 33));
  std::cout << f.get() << std::endl;
  return 100;
}

int main() {
  {
    auto f = pool.submit(print);
    f.get();
  }
  {
    auto f = pool.submit(print1);
    std::cout << f.get() << std::endl;
  }
  return 0;
}
