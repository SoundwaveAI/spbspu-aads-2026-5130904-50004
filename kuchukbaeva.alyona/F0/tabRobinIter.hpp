#ifndef TABROBINITER_HPP
#define TABROBINITER_HPP
#include <utility>
#include <cstddef>
#include "vec.hpp"

namespace kuchukbaeva
{
  template< class Key, class Value >
  struct HashNode
  {
    std::pair< Key, Value > kv;
    size_t psl = 0;
    bool isOccupied = false;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable;

  template< class Key, class Value, class Hash, class Equal >
  class HTCiter;

  template< class Key, class Value, class Hash, class Equal >
  class HTIter
  {
  public:
    using pair_t = std::pair< Key, Value >;
    using Node_t = HashNode< Key, Value >;

    HTIter();
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();
    HTIter& operator++();
    HTIter operator++(int);
    bool operator==(const HTIter& other) const;
    bool operator!=(const HTIter& other) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;
    friend class HTCiter< Key, Value, Hash, Equal >;
    HTIter(Vector< Node_t >* mass, size_t idx);
    Vector< Node_t >* mass_;
    size_t idx_;
  };

  template< class Key, class Value, class Hash, class Equal >
  class HTCiter
  {
  public:
    using pair_t = std::pair< Key, Value >;
    using Node_t = HashNode< Key, Value >;

    HTCiter();
    HTCiter(const HTIter< Key, Value, Hash, Equal >& other);
    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    HTCiter& operator++();
    HTCiter operator++(int);
    bool operator==(const HTCiter& other) const;
    bool operator!=(const HTCiter& other) const;

  private:
    friend class HashTable< Key, Value, Hash, Equal >;
    HTCiter(const Vector< Node_t >* mass, size_t idx);
    const Vector< Node_t >* mass_;
    size_t idx_;
  };
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >::HTIter():
  mass_(nullptr),
  idx_(0)
{}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >& kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator*()
{
  return mass_->operator[](idx_).kv;
}

template< class Key, class Value, class Hash, class Equal >
std::pair< Key, Value >* kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator->()
{
  return &(mass_->operator[](idx_).kv);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >&
  kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator++()
{
  if (!mass_)
  {
    return *this;
  }
  ++idx_;
  while (idx_ < mass_->getSize() && !(*mass_)[idx_].isOccupied)
  {
    ++idx_;
  }
  if (idx_ >= mass_->getSize())
  {
    mass_ = nullptr;
    idx_ = 0;
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
  return mass_ == other.mass_ && idx_ == other.idx_;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HTIter< Key, Value, Hash, Equal >::operator!=(const HTIter& other) const
{
  return !(*this == other);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::HTCiter():
  mass_(nullptr),
  idx_(0)
{}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::HTCiter(const HTIter< Key, Value, Hash, Equal >& other):
  mass_(other.mass_),
  idx_(other.idx_)
{}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >::HTIter(Vector< Node_t >* mass, size_t idx):
  mass_(mass),
  idx_(idx)
{}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::HTCiter(const Vector< Node_t >* mass, size_t idx):
  mass_(mass),
  idx_(idx)
{}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >& kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator*() const
{
  return mass_->operator[](idx_).kv;
}

template< class Key, class Value, class Hash, class Equal >
const std::pair< Key, Value >* kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator->() const
{
  return &(mass_->operator[](idx_).kv);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >&
  kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator++()
{
  if (!mass_)
  {
    return *this;
  }
  ++idx_;
  while (idx_ < mass_->getSize() && !(*mass_)[idx_].isOccupied)
  {
    ++idx_;
  }
  if (idx_ >= mass_->getSize())
  {
    mass_ = nullptr;
    idx_ = 0;
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
  return mass_ == other.mass_ && idx_ == other.idx_;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HTCiter< Key, Value, Hash, Equal >::operator!=(const HTCiter& other) const
{
  return !(*this == other);
}

#endif
