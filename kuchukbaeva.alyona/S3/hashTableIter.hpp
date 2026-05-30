#ifndef HASHTABLEITER_HPP
#define HASHTABLEITER_HPP

#include <utility>
#include <list.hpp>
#include "vector.hpp"

namespace kuchukbaeva
{

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTCiter;

  template< class Key, class Value, class Hash, class Equal >
  class HTIter
  {
  public:
    using pair_t = std::pair< Key, Value >;

    HTIter();

    pair_t& operator*();
    pair_t* operator->();

    HTIter& operator++();
    HTIter operator++(int);

    bool operator==(const HTIter& other) const;
    bool operator!=(const HTIter& other) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HTCiter< Key, Value, Hash, Equal >;
    HTIter(List< pair_t >* listsArray, size_t arraySize, size_t idx, LIter< pair_t > listIt);
    List< pair_t >* listsArray_;
    size_t arraySize_;
    size_t massIdx_;
    LIter< pair_t > listIt_;
  };


  template< class Key, class Value, class Hash, class Equal >
  class HTCiter {
  public:
    using pair_t = std::pair< Key, Value >;

    HTCiter();
    HTCiter(const HTIter< Key, Value, Hash, Equal >& other);

    const pair_t& operator*() const;
    const pair_t* operator->() const;

    HTCiter& operator++();
    HTCiter operator++(int);

    bool operator==(const HTCiter& other) const;
    bool operator!=(const HTCiter& other) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;
    HTCiter(const List< pair_t >* listsArray, size_t arraySize, size_t idx, LCIter< pair_t > listIt);
    const List< pair_t >* listsArray_;
    size_t arraySize_;
    size_t massIdx_;
    LCIter< pair_t > listIt_;
  };

}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >::HTIter():
  listsArray_(nullptr),
  arraySize_(0),
  massIdx_(0),
  listIt_()
{}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >& kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator*()
{
  return *listIt_;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >* kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator->()
{
  return &(*listIt_);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >&
  kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  if (listsArray_ && listIt_ == listsArray_[massIdx_].end())
  {
    ++massIdx_;
    while (massIdx_ < arraySize_ && listsArray_[massIdx_].isEmpty())
    {
      ++massIdx_;
    }
    if (massIdx_ < arraySize_)
    {
      listIt_ = listsArray_[massIdx_].begin();
    }
    else
    {
      listIt_ = LIter< std::pair< Key, Value > >();
    }
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >
  kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator++(int)
{
  HTIter tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator==(const HTIter& other) const
{
  return massIdx_ == other.massIdx_ && listIt_ == other.listIt_;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator!=(const HTIter& other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::HTCiter():
  listsArray_(nullptr),
  arraySize_(0),
  massIdx_(0),
  listIt_()
{}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::HTCiter(const HTIter< Key, Value, Hash, Equal >& other):
  listsArray_(other.listsArray_),
  arraySize_(other.arraySize_),
  massIdx_(other.massIdx_),
  listIt_(other.listIt_)
{}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >& kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator*() const
{
  return *listIt_;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >* kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator->() const
{
  return &(*listIt_);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >&
  kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator++()
{
  ++listIt_;
  if (listsArray_ && listIt_ == listsArray_[massIdx_].cend())
  {
    ++massIdx_;
    while (massIdx_ < arraySize_ && listsArray_[massIdx_].isEmpty())
    {
      ++massIdx_;
    }
    if (massIdx_ < arraySize_)
    {
      listIt_ = listsArray_[massIdx_].cbegin();
    }
    else
    {
      listIt_ = LCIter< std::pair< Key, Value > >();
    }
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >
  kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator++(int)
{
  HTCiter tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator==(const HTCiter& other) const
{
  return massIdx_ == other.massIdx_ && listIt_ == other.listIt_;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator!=(const HTCiter& other) const
{
  return !(*this == other);
}

#endif
