#include <iostream>
#include <string>

template <typename T>
using EnableIfString =
    std::enable_if_t<std::is_convertible<T, std::string>::value>;

class Person {
  std::string name;

public:
  template <typename STR, typename = EnableIfString<STR>>
  explicit Person(STR &&n) : name(std::forward<STR>(n)) {
    std::cout << "TMPL: " << name << '\n';
  }
  Person(const Person &p) : name(p.name) {
    std::cout << "COPY: " << name << '\n';
  }
  Person(Person &&p) : name(std::move(p.name)) {
    std::cout << "MOVE: " << name << '\n';
  }
};

