#include <iostream>
#include <tuple>
#include <functional>

template <typename T>
void print(T arg) {
  std::cout << arg << '\n';
}

template <typename T, typename... Types>
void print(T first, Types... args) {
  std::cout << "curr size: " << (sizeof...(args) + 1) << ": ";
  print(first);
  print(args...);
}

template <typename... Types>
void printDoubled(const Types&... args) {
  print(args + args...);
}

template <typename... Types>
void AddOne(const Types&... args) {
  print((args + 1)...);
}

template <typename ContainerType, typename... Idx>
void printElems(const ContainerType& c, Idx... idx) {
  print(c[idx]...);
}

template <std::size_t...>
struct Indices {};

template <typename T, std::size_t... Idx>
void printByIdx(T c, Indices<Idx...>) {
  print(std::get<Idx>(c)...);
}



