#ifndef LIST_ITERATORS_HPP
#define LIST_ITERATORS_HPP
#include <iterator>
#include <memory>
#include "node.hpp"

namespace  kuchukbaeva {
  template< class T > class List;
  template< class T > class LCIter;
  template< class T >
  class LIter: public std::iterator< std::forward_iterator_tag, T, std::ptrdiff_t, T*, T& >
  {
  public:
    LIter();

    T& operator*() const;
    T* operator->() const;

    LIter& operator++();
    LIter operator++(int);

    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;

  private:
    friend class List< T >;
    friend class LCIter< T >;
    explicit LIter(detail::Node< T >* node);
    detail::Node< T >* node_;
  };

  template< class T >
  class LCIter: public std::iterator< std::forward_iterator_tag, T, std::ptrdiff_t, const T*, const T& >
  {
  public:
    LCIter();
    LCIter(const LIter< T >& other);

    const T& operator*() const;
    const T* operator->() const;

    LCIter& operator++();
    LCIter operator++(int);

    bool operator==(const LCIter& other) const;
    bool operator!=(const LCIter& other) const;

  private:
    friend class List< T >;
    explicit LCIter(const detail::Node< T >* node);
    const detail::Node< T >* node_;
  };
}

template< class T >
kuchukbaeva::LIter< T >::LIter():
  node_(nullptr)
{}

template< class T >
kuchukbaeva::LIter< T >::LIter(kuchukbaeva::detail::Node< T >* node):
  node_(node)
{}

template< class T >
T& kuchukbaeva::LIter< T >::operator*() const
{
  return node_->data_;
}

template< class T >
T* kuchukbaeva::LIter< T >::operator->() const
{
  return std::addressof(node_->data_);
}

template< class T >
kuchukbaeva::LIter< T >& kuchukbaeva::LIter< T >::operator++()
{
  node_ = node_->next_;
  return *this;
}

template< class T >
kuchukbaeva::LIter< T > kuchukbaeva::LIter< T >::operator++(int)
{
  LIter tmp = *this;
  node_ = node_->next_;
  return tmp;
}

template< class T >
bool kuchukbaeva::LIter< T >::operator==(const LIter& other) const
{
  return node_ == other.node_;
}

template< class T >
bool kuchukbaeva::LIter< T >::operator!=(const LIter& other) const
{
  return node_ != other.node_;
}

template< class T >
kuchukbaeva::LCIter< T >::LCIter():
  node_(nullptr)
{}

template< class T >
kuchukbaeva::LCIter< T >::LCIter(const kuchukbaeva::detail::Node< T >* node):
  node_(node)
{}

template< class T >
kuchukbaeva::LCIter< T >::LCIter(const kuchukbaeva::LIter< T >& other):
  node_(other.node_)
{}

template< class T >
const T& kuchukbaeva::LCIter< T >::operator*() const
{
  return node_->data_;
}

template< class T >
const T* kuchukbaeva::LCIter< T >::operator->() const
{
  return &node_->data_;
}

template< class T >
kuchukbaeva::LCIter< T >& kuchukbaeva::LCIter< T >::operator++()
{
  node_ = node_->next_;
  return *this;
}

template< class T >
kuchukbaeva::LCIter< T > kuchukbaeva::LCIter< T >::operator++(int)
{
  LCIter tmp = *this;
  node_ = node_->next_;
  return tmp;
}

template< class T >
bool kuchukbaeva::LCIter< T >::operator==(const LCIter& other) const
{
  return node_ == other.node_;
}

template< class T >
bool kuchukbaeva::LCIter< T >::operator!=(const LCIter& other) const
{
  return node_ != other.node_;
}

#endif
