#include <deque>

template <typename T, template <typename Elem, typename = std::allocator<Elem>>
                      class Cont = std::deque>
class Stack {
 private:
  Cont<T> elems;

 public:
  void push(const T&);
  void pop();
  const T& top() const;
  bool empty() const { return elems.empty(); }

  template <typename T2,
            template <typename Elem2, typename = std::allocator<Elem2>>
            class Cont2>
  Stack<T, Cont>& operator=(const Stack<T2, Cont2>&);

  template <typename, template <typename, typename> class>
  friend class Stack;
};

template <typename T, template <typename, typename> class Cont>
void Stack<T, Cont>::push(const T& t) {
  elems.push_back(t);
}

template <typename T, template <typename, typename> class Cont>
void Stack<T, Cont>::pop() {
  elems.pop_back();
}

template <typename T, template <typename, typename> class Cont>
const T& Stack<T, Cont>::top() const {
  return elems.back();
}

template <typename T, template <typename, typename> class Cont>
template <typename T2, template <typename, typename> class Cont2>
Stack<T, Cont>& Stack<T, Cont>::operator=(const Stack<T2, Cont2>& other) {
  elems.clear();
  elems.insert(elems.begin(), other.elems.begin(), other.elems.end());
  return *this;
}
