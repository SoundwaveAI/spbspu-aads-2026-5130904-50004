#ifndef TAB_HPP
#define TAB_HPP
#include <utility>
#include <stdexcept>
#include <string>
#include "vec.hpp"
#include "tabRobinIter.hpp"

namespace kuchukbaeva
{
  template< class T >
  struct DefaultHash
  {
    std::size_t operator()(const T& val) const
    {
      return std::hash< T >{}(val);
    }
  };

  template< class T >
  struct EqualFunctor
  {
    bool operator()(const T& lhs, const T& rhs) const
    {
      return lhs == rhs;
    }
  };

  template< class Key, class Value, class Hash = DefaultHash< Key >, class Equal = EqualFunctor< Key > >
  class HashTable
  {
  public:
    using Node = HashNode< Key, Value >;

    explicit HashTable(size_t slots = 16);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other);
    ~HashTable() = default;

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    void swap(HashTable& other) noexcept;
    void add(const Key& k, const Value& v);
    bool drop(const Key& k, Value& outValue);
    bool has(const Key& k) const;
    HTIter< Key, Value, Hash, Equal > find(const Key& k);
    HTCiter< Key, Value, Hash, Equal > find(const Key& k) const;
    void rehash(size_t slots);
    size_t getSize() const;

    HTIter< Key, Value, Hash, Equal > begin();
    HTIter< Key, Value, Hash, Equal > end();

    HTCiter< Key, Value, Hash, Equal > begin() const;
    HTCiter< Key, Value, Hash, Equal > end() const;
    HTCiter< Key, Value, Hash, Equal > cbegin() const;
    HTCiter< Key, Value, Hash, Equal > cend() const;

  private:
    friend class HTIter< Key, Value, Hash, Equal >;
    friend class HTCiter< Key, Value, Hash, Equal >;
    void add_impl(const Key& k, const Value& v);
    bool drop_impl(const Key& k, Value& outValue);
    Vector< Node > mass_;
    size_t size_;
    Hash hashFn_;
    Equal equalFn_;
    const float maxLoadFactor_ = 0.7f;
  };
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
  mass_(slots),
  size_(0),
  hashFn_(),
  equalFn_()
{
  for (size_t i = 0; i < slots; ++i)
  {
    mass_.pushBack(Node{ std::make_pair(Key(), Value()), 0, false });
  }
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
  mass_(other.mass_),
  size_(other.size_),
  hashFn_(other.hashFn_),
  equalFn_(other.equalFn_)
{}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other):
  mass_(),
  size_(0),
  hashFn_(std::move(other.hashFn_)),
  equalFn_(std::move(other.equalFn_))
{
  swap(other);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HashTable< Key, Value, Hash, Equal >&
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
{
  if (this != std::addressof(other))
  {
    HashTable tmp(other);
    swap(tmp);
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HashTable< Key, Value, Hash, Equal >&
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
{
  if (this != std::addressof(other))
  {
    HashTable tmp(std::move(other));
    swap(tmp);
  }
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void kuchukbaeva::HashTable< Key, Value, Hash, Equal >::swap(HashTable& other) noexcept
{
  mass_.swap(other.mass_);
  std::swap(size_, other.size_);
  std::swap(hashFn_, other.hashFn_);
  std::swap(equalFn_, other.equalFn_);
}

template< class Key, class Value, class Hash, class Equal >
void kuchukbaeva::HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
{
  HashTable tmp(*this);
  tmp.add_impl(k,v);
  swap(tmp);
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HashTable< Key, Value, Hash, Equal >::drop(const Key& k, Value& outValue)
{
  if (mass_.isEmpty())
  {
    return false;
  }
  size_t idx = hashFn_(k) % mass_.getSize();
  size_t startIdx = idx;
  bool found = false;
  while (mass_[idx].isOccupied)
  {
    if (equalFn_(mass_[idx].kv.first, k))
    {
      found = true;
      break;
    }
    idx = (idx + 1) % mass_.getSize();
    if (idx == startIdx)
    {
      break;
    }
  }
  if (!found)
  {
    return false;
  }
  outValue = mass_[idx].kv.second;
  HashTable tmp(*this);
  Value dummy;
  tmp.drop_impl(k, dummy);
  swap(tmp);
  return true;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HashTable< Key, Value, Hash, Equal >::has(const Key& k) const
{
  if (mass_.isEmpty())
  {
    return false;
  }
  size_t idx = hashFn_(k) % mass_.getSize();
  size_t currentPsl = 0;

  while (mass_[idx].isOccupied)
  {
    if (currentPsl > mass_[idx].psl)
    {
      return false;
    }
    if (equalFn_(mass_[idx].kv.first, k))
    {
      return true;
    }

    idx = (idx + 1) % mass_.getSize();
    ++currentPsl;
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::find(const Key& k)
{
  if (mass_.isEmpty())
  {
    return end();
  }
  size_t idx = hashFn_(k) % mass_.getSize();
  size_t currentPsl = 0;

  while (mass_[idx].isOccupied)
  {
    if (currentPsl > mass_[idx].psl)
    {
      return end();
    }
    if (equalFn_(mass_[idx].kv.first, k))
    {
      return HTIter< Key, Value, Hash, Equal >(&mass_, idx);
    }
    idx = (idx + 1) % mass_.getSize();
    ++currentPsl;
  }
  return end();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
{
  if (mass_.isEmpty())
  {
    return end();
  }
  size_t idx = hashFn_(k) % mass_.getSize();
  size_t currentPsl = 0;

  while (mass_[idx].isOccupied)
  {
    if (currentPsl > mass_[idx].psl)
    {
      return end();
    }
    if (equalFn_(mass_[idx].kv.first, k))
    {
      return HTCiter< Key, Value, Hash, Equal >(&mass_, idx);
    }
    idx = (idx + 1) % mass_.getSize();
    ++currentPsl;
  }
  return end();
}

template< class Key, class Value, class Hash, class Equal >
void kuchukbaeva::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  if (slots == 0)
  {
    throw std::invalid_argument("Slots cannot be zero");
  }
  HashTable tmp(slots);
  for (size_t i = 0; i < mass_.getSize(); ++i)
  {
    if (mass_[i].isOccupied)
    {
      tmp.add_impl(mass_[i].kv.first, mass_[i].kv.second);
    }
  }
  swap(tmp);
}

template< class Key, class Value, class Hash, class Equal >
size_t kuchukbaeva::HashTable< Key, Value, Hash, Equal >::getSize() const
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::begin()
{
  for (size_t i = 0; i < mass_.getSize(); ++i)
  {
    if (mass_[i].isOccupied)
    {
      return HTIter< Key, Value, Hash, Equal >(&mass_, i);
    }
  }
  return end();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::end()
{
  return HTIter< Key, Value, Hash, Equal >(nullptr, 0);
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::end() const
{
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  for (size_t i = 0; i < mass_.getSize(); ++i)
  {
    if (mass_[i].isOccupied)
    {
      return HTCiter< Key, Value, Hash, Equal >(&mass_, i);
    }
  }
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return HTCiter< Key, Value, Hash, Equal >(nullptr, 0);
}

template< class Key, class Value, class Hash, class Equal >
void kuchukbaeva::HashTable< Key, Value, Hash, Equal >::add_impl(const Key& k, const Value& v)
{
  if (mass_.isEmpty() || static_cast< float >(size_ + 1) / mass_.getSize() > maxLoadFactor_)
  {
    rehash(mass_.isEmpty() ? 16 : mass_.getSize() * 2);
  }
  Node current{ std::make_pair(k, v), 0, true };
  size_t idx = hashFn_(k) % mass_.getSize();
  bool swapped = false;
  while (true)
  {
    if (!swapped && mass_[idx].isOccupied && equalFn_(mass_[idx].kv.first, current.kv.first))
    {
      mass_[idx].kv.second = current.kv.second;
      return;
    }
    if (!mass_[idx].isOccupied)
    {
      mass_[idx] = std::move(current);
      ++size_;
      return;
    }
    if (current.psl > mass_[idx].psl)
    {
      std::swap(current, mass_[idx]);
      swapped = true;
    }
    idx = (idx + 1) % mass_.getSize();
    ++current.psl;
  }
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HashTable< Key, Value, Hash, Equal >::drop_impl(const Key& k, Value& outValue)
{
  if (mass_.isEmpty())
  {
    return false;
  }
  size_t idx = hashFn_(k) % mass_.getSize();
  size_t startIdx = idx;

  while (mass_[idx].isOccupied)
  {
    if (equalFn_(mass_[idx].kv.first, k))
    {
      break;
    }
    idx = (idx + 1) % mass_.getSize();
    if (idx == startIdx)
    {
      return false;
    }
  }
  if (!mass_[idx].isOccupied)
  {
    return false;
  }
  outValue = mass_[idx].kv.second;
  size_t nextIdx = (idx + 1) % mass_.getSize();
  while (mass_[nextIdx].isOccupied && mass_[nextIdx].psl > 0)
  {
    mass_[idx] = std::move(mass_[nextIdx]);
    --mass_[idx].psl;
    idx = nextIdx;
    nextIdx = (nextIdx + 1) % mass_.getSize();
  }
  mass_[idx] = Node{ std::make_pair(Key(), Value()), 0, false };
  --size_;
  return true;
}

#endif
