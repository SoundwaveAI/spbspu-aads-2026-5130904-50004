#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include <list.hpp>

namespace kuchukbaeva
{
  template< class T >
  class Queue
  {
  public:
    void push(const T& rhs);
    void pop();
    T& top();
    const T& top() const;
    bool empty() const;
    size_t getSize() const;
    void clear();

  private:
    List< T > list_{};
    LIter< T > tail_ = list_.beforeBegin();
    size_t size_ = 0;
  };
}

template< class T >
void kuchukbaeva::Queue< T >::push(const T& rhs)
{
  LIter< T > new_tail = list_.insertAfter(tail_, rhs);
  tail_ = new_tail;
  ++size_;
}

template< class T >
void kuchukbaeva::Queue< T >::pop()
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  list_.pop_front();
  --size_;
  if (empty())
  {
    tail_ = list_.beforeBegin();
  }
}

template< class T >
T& kuchukbaeva::Queue< T >::top()
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  return list_.front();
}

template< class T >
const T& kuchukbaeva::Queue< class T >::top() const
{
  if (empty())
  {
    throw std::out_of_range("Queue is empty");
  }
  return list_.front();
}

template< class T >
bool kuchukbaeva::Queue< class T >::empty() const
{
  return list_.isEmpty();
}

template< class T >
size_t kuchukbaeva::Queue< T >::getSize() const
{
  return size_;
}

template < class T >
void kuchukbaeva::Queue< T >::clear()
{
  list_.clear();
  tail_ = list_.beforeBegin();
  size_ = 0;
}

#endif
