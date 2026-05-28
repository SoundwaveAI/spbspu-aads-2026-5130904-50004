#ifndef LIST_HPP
#define LIST_HPP
#include <stdexcept>
#include <utility>
#include "list-iterators.hpp"

namespace kuchukbaeva {

  template< class T >
  class List
  {
  public:
    List();
    List(const List& other);
    List(List&& other) noexcept;
    ~List();

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    LIter< T > begin();
    LIter< T > end();
    LCIter< T > cbegin() const;
    LCIter< T > cend() const;
    LIter< T > beforeBegin();

    T& front();
    const T& front() const;

    void push_front(const T& value);
    void pop_front();

    void swap(List& other) noexcept;
    void clear();
    bool isEmpty() const;

    LIter< T > insertAfter(LIter< T > pos, const T& value);
    LIter< T > eraseAfter(LIter< T > pos);

  private:
    detail::Node< T >* fake_;
  };
}

template< class T >
kuchukbaeva::List< T >::List():
  fake_(new kuchukbaeva::detail::Node< T >())
{}

template< class T >
kuchukbaeva::List< T >::List(const List& other):
  List()
{
  LIter< T > it = beforeBegin();
  for (LCIter< T > otherIt = other.cbegin(); otherIt != other.cend(); ++otherIt)
  {
    it = insertAfter(it, *otherIt);
  }
}

template< class T >
kuchukbaeva::List< T >::List(List&& other) noexcept:
  List()
{
  swap(other);
}

template< class T >
kuchukbaeva::List< T >::~List()
{
  clear();
  delete fake_;
}

template< class T >
kuchukbaeva::List< T >& kuchukbaeva::List< T >::operator=(const List& other)
{
  if (this != &other)
  {
    List< T > temp(other);
    swap(temp);
  }
  return *this;
}

template< class T >
kuchukbaeva::List< T >& kuchukbaeva::List< T >::operator=(List&& other) noexcept
{
  if (this != &other) {
    swap(other);
  }
  return *this;
}

template< class T >
kuchukbaeva::LIter< T > kuchukbaeva::List< T >::begin()
{
  return LIter< T >(fake_->next_);
}

template< class T >
kuchukbaeva::LIter< T > kuchukbaeva::List< T >:: end()
{
  return LIter< T >(fake_);
}

template< class T >
kuchukbaeva::LCIter< T > kuchukbaeva::List< T >::cbegin() const
{
  return LCIter< T >(fake_->next_);
}

template< class T >
kuchukbaeva::LCIter< T > kuchukbaeva::List< T >::cend() const
{
  return LCIter< T >(fake_);
}

template< class T >
kuchukbaeva::LIter< T > kuchukbaeva::List< T >::beforeBegin()
{
  return LIter< T >(fake_);
}

template< class T >
T& kuchukbaeva::List< T >::front()
{
  return fake_->next_->data_;
}

template< class T >
const T& kuchukbaeva::List< T >::front() const
{
  return fake_->next_->data_;
}

template< class T >
void kuchukbaeva::List< T >::push_front(const T& value)
{
  insertAfter(beforeBegin(), value);
}

template< class T >
void kuchukbaeva::List< T >::pop_front()
{
  if (isEmpty())
  {
    throw std::out_of_range("List is empty");
  }
  eraseAfter(beforeBegin());
}

template< class T >
void kuchukbaeva::List< T >::swap(List& other) noexcept
{
  std::swap(fake_, other.fake_);
}

template< class T >
void kuchukbaeva::List< T >::clear()
{
  while (!isEmpty())
  {
    pop_front();
  }
}

template< class T >
bool kuchukbaeva::List< T >::isEmpty() const
{
  return fake_ ? fake_->next_ == fake_ : true;
}

template< class T >
kuchukbaeva::LIter< T > kuchukbaeva::List< T >::insertAfter(LIter< T > pos, const T& value)
{
  kuchukbaeva::detail::Node< T >* newNode = new kuchukbaeva::detail::Node< T >(std::move(value), pos.node_->next_);
  pos.node_->next_ = newNode;
  return LIter< T >(newNode);
}

template< class T >
kuchukbaeva::LIter< T > kuchukbaeva::List< T >::eraseAfter(LIter< T > pos)
{
  kuchukbaeva::detail::Node< T >* toDelete = pos.node_->next_;
  pos.node_->next_ = toDelete->next_;
  delete toDelete;
  return LIter< T >(pos.node_->next_);
}

#endif
