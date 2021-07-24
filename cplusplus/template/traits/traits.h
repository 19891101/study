#include <iostream>
#include <type_traits>

template <class...>
using my_void = void;

#define DEFINE_HAS_MEMBER(Member)                              \
  template <typename, typename = my_void<>>                    \
  struct HasMemberT_##Member : std::false_type {};             \
  template <typename T>                                        \
  struct HasMemberT_##Member<T, my_void<decltype(&T::Member)>> \
      : std::true_type {};

DEFINE_HAS_MEMBER(member_a);
DEFINE_HAS_MEMBER(member_b);

template <typename T, std::enable_if_t<HasMemberT_member_a<T>::value, int> = 0>
void Set(T &t) {
  std::cout << "Has member a" << '\n';
  t.member_a = 1;
}

template <typename T, std::enable_if_t<HasMemberT_member_b<T>::value, int> = 0>
void Set(T &t) {
  std::cout << "Has member b" << '\n';
  t.member_b = 2;
}

template <typename T, std::enable_if_t<!HasMemberT_member_a<T>::value &&
                                           !HasMemberT_member_b<T>::value,
                                       int> = 0>
void Set(T &t) {
  std::cout << "Has nothing" << '\n';
}

