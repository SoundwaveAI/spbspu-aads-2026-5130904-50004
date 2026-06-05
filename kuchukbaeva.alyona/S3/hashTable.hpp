#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <utility>
#include <stdexcept>
#include <string>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <list.hpp>
#include "vector.hpp"
#include "hashTableIter.hpp"

namespace kuchukbaeva
{
  template< class T >
  struct SipHashFunctor
  {
    std::size_t operator()(const T& val) const
    {
      boost::hash2::siphash_64 hasher;
      boost::hash2::hash_append(hasher, {}, val);
      return hasher.result();
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

  template< class Key, class Value, class Hash = SipHashFunctor< Key >, class Equal = EqualFunctor< Key > >
  class HashTable
  {
  public:
    explicit HashTable(size_t slots = 16);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other);
    ~HashTable() = default;

    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    void swap(HashTable& other) noexcept;

    void add(const Key& k, const Value& v);
    bool drop(const Key& k);
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
    Vector< List< std::pair< Key, Value > > > mass_;
    size_t size_;
    Hash hashFn_;
    Equal equalFn_;
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
    mass_.pushBack(List< std::pair< Key, Value > >());
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
  if (this != &other)
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
  if (this != &other)
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
}

template< class Key, class Value, class Hash, class Equal >
void kuchukbaeva::HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
{
  size_t idx = hashFn_(k) % mass_.getSize();
  for (LIter< std::pair< Key, Value > > it = mass_[idx].begin(); it != mass_[idx].end(); ++it)
  {
    if (equalFn_(it->first, k))
    {
      it->second = v;
      return;
    }
  }
  mass_[idx].push_front(std::make_pair(k, v));
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HashTable< Key, Value, Hash, Equal >::drop(const Key& k)
{
  if (mass_.getSize() == 0)
  {
    return false;
  }
  size_t idx = hashFn_(k) % mass_.getSize();
  LIter< std::pair< Key, Value > > beforeIt = mass_[idx].beforeBegin();
  LIter< std::pair< Key, Value > > it = mass_[idx].begin();
  while (it != mass_[idx].end())
  {
    if (equalFn_(it->first, k))
    {
      it = mass_[idx].eraseAfter(beforeIt);
      --size_;
      return true;
    }
    else
    {
      ++it;
      ++beforeIt;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
bool kuchukbaeva::HashTable< Key, Value, Hash, Equal >::has(const Key& k) const
{
  size_t idx = hashFn_(k) % mass_.getSize();
  for (LCIter< std::pair< Key, Value > > it = mass_[idx].cbegin(); it != mass_[idx].cend(); ++it)
  {
    if (equalFn_(it->first, k))
    {
      return true;
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::find(const Key& k)
{
  size_t idx = hashFn_(k) % mass_.getSize();
  for (LIter< std::pair< Key, Value > > it = mass_[idx].begin(); it != mass_[idx].end(); ++it)
  {
    if (equalFn_(it->first, k))
    {
      return HTIter< Key, Value, Hash, Equal >(&mass_[0], mass_.getSize(), idx, it);
    }
  }
  return end();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal >
  kuchukbaeva::HashTable< Key, Value, Hash, Equal >::find(const Key& k) const
{
  size_t idx = hashFn_(k) % mass_.getSize();
  for (LCIter< std::pair< Key, Value > > it = mass_[idx].cbegin(); it != mass_[idx].cend(); ++it)
  {
    if (equalFn_(it->first, k))
    {
      return HTCiter< Key, Value, Hash, Equal >(&mass_[0], mass_.getSize(), idx, it);
    }
  }
  return cend();
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
    for (LIter< std::pair< Key, Value > > it = mass_[i].begin(); it != mass_[i].end(); ++it)
    {
      tmp.add(it->first, it->second);
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
  if (mass_.isEmpty())
  {
    return end();
  }
  for (size_t i = 0; i < mass_.getSize(); ++i)
  {
    if (!mass_[i].isEmpty())
    {
      return HTIter< Key, Value, Hash, Equal >(&mass_[0], mass_.getSize(), i, mass_[i].begin());
    }
  }
  return end();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTIter< Key, Value, Hash, Equal > kuchukbaeva::HashTable< Key, Value, Hash, Equal >::end()
{
  return HTIter< Key, Value, Hash, Equal >(nullptr, mass_.getSize(),
    mass_.getSize(), LIter< std::pair< Key, Value > >());
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal > kuchukbaeva::HashTable< Key, Value, Hash, Equal >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal > kuchukbaeva::HashTable< Key, Value, Hash, Equal >::end() const
{
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal > kuchukbaeva::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  for (size_t i = 0; i < mass_.getSize(); ++i)
  {
    if (!mass_[i].isEmpty())
    {
      return HTCiter< Key, Value, Hash, Equal >(&mass_[0], mass_.getSize(), i, mass_[i].cbegin());
    }
  }
  return cend();
}

template< class Key, class Value, class Hash, class Equal >
kuchukbaeva::HTCiter< Key, Value, Hash, Equal > kuchukbaeva::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return HTCiter< Key, Value, Hash, Equal >(nullptr, mass_.getSize(),
    mass_.getSize(), LCIter< std::pair< Key, Value > >());
}

#endif
