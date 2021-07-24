#include "traits.h"

struct Type1 {
  int member_a;
};

struct Type2 {
  int member_b;
};

struct Type3 {
};

int main() {
  Type1 t1;
  Type2 t2;
  Type3 t3;
  Set(t1);
  Set(t2);
  Set(t3);
  std::cout << t1.member_a << ", " << t2.member_b << '\n';
  return 0;
}

