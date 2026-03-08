#ifndef LIST_HPP
#define LIST_HPP

#include <utility>

namespace kuchukbaeva {

  template < class T >
  class List;

  template < class T >
  class LIter;

  template < class T >
  class LCIter;

  namespace detail {
    template < class T >
    class Node {
    public:
      Node():
        data_(),
        next_(this)
      {
      }

      Node(const T& d, Node* n):
        data_(d),
        next_(n)
      {
      }

      T data_;
      Node* next_;
    };
  }

  template < class T >
  class LIter {
  public:
    LIter(detail::Node< T >* node = nullptr):
      node_(node)
    {
    }

    T& operator*()
    {
      return node_->data_;
    }

    LIter& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LIter operator++(int)
    {
      LIter tmp = *this;
      node_ = node_->next_;
      return tmp;
    }

    bool operator==(const LIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LIter& other) const
    {
      return node_ != other.node_;
    }

  private:
    friend class List< T >;
    friend class LCIter< T >;
    detail::Node< T >* node_;
  };
  template < class T >
  class LCIter {
  public:
    LCIter(const detail::Node< T >* node = nullptr):
      node_(node)
    {
    }

    LCIter(const LIter< T >& other):
      node_(other.node_)
    {
    }

    const T& operator*() const
    {
      return node_->data_;
    }

    LCIter& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter tmp = *this;
      node_ = node_->next_;
      return tmp;
    }

    bool operator==(const LCIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LCIter& other) const
    {
      return node_ != other.node_;
    }

  private:
    friend class List< T >;
    const detail::Node< T >* node_;
  };
}
