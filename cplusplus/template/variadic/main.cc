#include <array>
#include <string>
#include <vector>

#include "variadic.h"

int main() {
  std::string s("second");
  print(12, "first", 12.34, s);
  printDoubled(12, s, 12.34);
  AddOne(12, 12.34);
  std::vector<std::string> c = {"first", "second", "third"};
  printElems(c, 2, 1, 0);
  std::array<std::string, 3> a = {"first", "second", "third"};
  printByIdx(a, Indices<2, 1, 0>());
  return 0;
}

