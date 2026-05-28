#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace kuchukbaeva {
  namespace detail {
    template< class T >
    struct Node
    {
      Node();
      Node(const T& d, Node* n);
      Node(T&& d, Node* n);

      T data_;
      Node* next_;
    };
  }
}

template< class T >
kuchukbaeva::detail::Node< T >::Node():
  data_(),
  next_(this)
{}

template< class T >
kuchukbaeva::detail::Node< T >::Node(const T& d, Node* n):
  data_(d),
  next_(n)
{}

template< class T >
kuchukbaeva::detail::Node< T >::Node(T&& d, Node* n):
  data_(std::move(d)),
  next_(n)
{}

#endif
