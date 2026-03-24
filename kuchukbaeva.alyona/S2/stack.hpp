#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "../common/list.hpp"

namespace kuchukbaeva {
  template < class T >
  class Stack {
  public:
    void push(const T& rhs) {
      list_.push_front(rhs);
    }
    const T& top() const {
      if (list_.isEmpty()) {
        throw std::out_of_range("Stack is empty");
      }
      return list_.front();
    }

    void drop() {
      if (list_.isEmpty()) {
        throw std::out_of_range("Stack is empty");
      }
      list_.pop_front();
    }

    bool isEmpty() const {
      return list_.isEmpty();
    }

  private:
    List< T > list_;
  };
}

#endif
