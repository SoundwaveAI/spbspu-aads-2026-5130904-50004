#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include <list.hpp>

namespace kuchukbaeva
{
  template< class T >
  class Stack
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
    size_t size_ = 0;
  };
}

template< class T >
void kuchukbaeva::Stack< T >::push(const T& rhs)
{
  list_.push_front(rhs);
  ++size_;
}

template< class T >
void kuchukbaeva::Stack< T >::pop()
{
  if (empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  list_.pop_front();
  --size_;
}

template< class T >
T& kuchukbaeva::Stack< T >::top()
{
  if (empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  return list_.front();
}

template< class T >
const T& kuchukbaeva::Stack< T >::top() const
{
  if (empty())
  {
    throw std::out_of_range("Stack is empty");
  }
  return list_.front();
}

template< class T >
bool kuchukbaeva::Stack< T >::empty() const
{
  return list_.isEmpty();
}

template< class T >
size_t kuchukbaeva::Stack< T >::getSize() const
{
  return size_;
}

template< class T >
void kuchukbaeva::Stack< T >::clear()
{
  list_.clear();
  size_ = 0;
}

#endif
