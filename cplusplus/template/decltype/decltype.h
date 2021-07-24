#include <functional>
#include <iostream>
#include <type_traits>

template <typename T, unsigned N> std::size_t len(T (&)[N]) {
  std::cout << "array called\n";
  return N;
}

/*
  decltype会返回最后一个参数的类型
  t.size()之所以加void, 是为了防止t.size()的返回值重载了逗号表达式
*/
template <typename T>
auto len(const T &t) -> decltype((void)(t.size()), typename T::size_type()) {
  std::cout << "size called\n";
  return t.size();
}

std::size_t len(...) { return 0; }

template <unsigned p, unsigned d> struct DoIsPrime {
  static constexpr bool value = (p % d != 0) && DoIsPrime<p, d - 1>::value;
};

template <unsigned p> struct DoIsPrime<p, 2> {
  static constexpr bool value = p % 2 != 0;
};

template <unsigned p> struct IsPrime {
  static constexpr bool value = DoIsPrime<p, p / 2>::value;
};

template <> struct IsPrime<0> { static constexpr bool value = false; };
template <> struct IsPrime<1> { static constexpr bool value = false; };
template <> struct IsPrime<2> { static constexpr bool value = true; };
template <> struct IsPrime<3> { static constexpr bool value = true; };

