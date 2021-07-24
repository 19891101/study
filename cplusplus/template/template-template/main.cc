#include <iostream>
#include <vector>

#include "stack.h"

int main() {
  Stack<int> istack;
  Stack<double> dstack;
  istack.push(1);
  istack.push(2);
  dstack.push(12.2);
  dstack.push(101.5);
  std::cout << "dstack top: " << dstack.top() << "\n";
  istack = dstack;
  while (!istack.empty()) {
    std::cout << istack.top() << " ";
    istack.pop();
  }
  std::cout << "\n";
  Stack<int, std::vector> istack2;
  istack.push(12);
  return 0;
}

