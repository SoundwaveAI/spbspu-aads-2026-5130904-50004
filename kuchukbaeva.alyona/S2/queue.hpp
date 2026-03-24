#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include "../common/list.hpp"

namespace kuchukbaeva {
  template < class T >
  class Queue {
  public:
    void push(const T& rhs) {
      LIter< T > it = list_.beforeBegin();
      LIter< T > next = list_.begin();
      while (next != list_.end()) {
        ++it;
        ++next;
      }
      list_.insertAfter(it, rhs);
    }

    const T& top() const {
      if (list_.isEmpty()) {
        throw std::out_of_range("Queue is empty");
      }
      return list_.front();
    }

    void drop() {
      if (list_.isEmpty()) {
        throw std::out_of_range("Queue is empty");
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
