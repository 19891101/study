#include <functional>
#include <iostream>
#include <type_traits>

template <typename T, typename = std::enable_if_t<std::is_array<T>::value>>
void OutArray(T &arg) {
  std::cout << "is array: " << std::extent<T>::value << "\n";
}

template <typename T, typename = std::enable_if_t<!std::is_const<T>::value>>
void OutNotConst(T &arg) {
  std::cout << "not const\n";
}

void PrintString(const std::string &s) { std::cout << "[" << s << "]\n"; }

template <typename T> void PrintT(T arg) {
  std::cout << "template called\n";
  PrintString(arg);
}

void PrintT(std::reference_wrapper<const std::string> arg) {
  std::cout << "non-template called\n";
  PrintString(arg);
}

template <typename T>
auto ReturnT(T p) { // auto will remove const and reference
  return p;
}

template <typename T1, typename T2>
constexpr std::pair<typename std::decay<T1>::type, /*decay to remove const and*/
                    typename std::decay<T2>::type> /*reference*/
MyMakePair(T1 &&a, T2 &&b) {
  return std::pair<typename std::decay<T1>::type,
                   typename std::decay<T2>::type>(std::forward<T1>(a),
                                                  std::forward<T2>(b));
}

