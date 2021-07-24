#include "enable_if.h"

int main() {
  std::string s = "sname";
  Person p1(s);
  Person p2("tmp");
  Person p3(p1);
  Person p4(std::move(p1));
  return 0;
}

