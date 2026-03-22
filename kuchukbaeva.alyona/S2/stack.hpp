#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "../common/list.hpp"

namespace kuchukbaeva {
  template < class T >
  class Stack {
  public:
    void push(const T& rhs)
    {
      list_.push_front(rhs);
    }

    T drop()
    {
      if (list_.isEmpty()) {
        throw std::out_of_range("Stack is empty");
      }
      T val = list_.front();
      list_.pop_front();
      return val;
    }

    bool isEmpty() const
    {
      return list_.isEmpty();
    }

  private:
    List< T > list_;
  };
}

#endif
