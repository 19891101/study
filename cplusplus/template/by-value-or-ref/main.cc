#include "value_or_ref.h"
#include <vector>

struct C {};

int main() {
  int a[4];
  std::string s = "test";
  C c;
  const C cc;
  // OutArray(c);
  // OutArray(cc);
  OutArray("hi");
  // OutArray(s);
  OutArray(a);

  OutNotConst(c);
  // OutNotConst(cc);

  PrintT(std::cref(s));

  const std::string cs = "const test";
  auto rcs = ReturnT(cs);
  rcs += cs;
  std::cout << rcs << "\n";

  std::string &rs = s;
  std::vector<std::string> v;
  v.push_back(ReturnT(rs));

  const int x = 100;

  auto p = MyMakePair(x, rs);
  p.first *= 2;
  p.second += " append";
  std::cout << p.first << ", " << p.second << ", " << s << "\n";

  return 0;
}

