#ifndef VEC_HPP
#define VEC_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <new>
#include "vecIter.hpp"

namespace kuchukbaeva
{
  template< class T >
  class Vector
  {
  public:
    ~Vector();
    Vector() noexcept;
    explicit Vector(size_t size);
    Vector(const Vector< T >& rhs);
    Vector(Vector< T >&& rhs) noexcept;

    Vector< T >& operator=(const Vector< T >& rhs);
    Vector< T >& operator=(Vector< T >&& rhs) noexcept;
    void swap(Vector < T >& rhs) noexcept;

    void insert(size_t id, const T& value);
    void erase(size_t id);

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void pushBack(const T& value);
    void popBack();
    void reserve(size_t newCap);
    void pushBack(T&& value);

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    Viter< T > begin() noexcept;
    Viter< T > end() noexcept;
    Vciter< T > begin() const noexcept;
    Vciter< T > end() const noexcept;
    Vciter< T > cbegin() const noexcept;
    Vciter< T > cend() const noexcept;

  private:
    T* data_;
    size_t size_;
    size_t capacity_;
    void clear() noexcept;
  };
}

template< class T >
kuchukbaeva::Vector< T >::Vector() noexcept:
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template< class T >
kuchukbaeva::Vector< T >::Vector(size_t size):
  data_(size ? static_cast< T* >(::operator new(size * sizeof(T))) : nullptr),
  size_(0),
  capacity_(size)
{}

template< class T >
kuchukbaeva::Vector< T >::~Vector()
{
  clear();
  ::operator delete(data_);
}

template< class T >
void kuchukbaeva::Vector< T >::clear() noexcept
{
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i].~T();
  }
  size_ = 0;
}

template< class T >
kuchukbaeva::Vector< T >::Vector(Vector < T >&& rhs) noexcept:
  Vector()
{
  swap(rhs);
}

template< class T >
kuchukbaeva::Vector< T >& kuchukbaeva::Vector < T >::operator=(Vector< T >&& rhs) noexcept
{
  if (this != std::addressof(rhs))
  {
    clear();
    ::operator delete(data_);
    data_ = nullptr;
    capacity_ = 0;
    swap(rhs);
  }
  return *this;
}

template< class T >
T& kuchukbaeva::Vector< T >::operator[](size_t id) noexcept
{
  return data_[id];
}

template< class T >
const T& kuchukbaeva::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
kuchukbaeva::Vector< T >::Vector(const Vector< T >& rhs):
  data_(rhs.capacity_ ? static_cast< T* >(::operator new(rhs.capacity_ * sizeof(T))) : nullptr),
  size_(0),
  capacity_(rhs.capacity_)
{
  try
  {
    for (size_t i = 0; i < rhs.size_; ++i)
    {
      new (data_ + i) T(rhs.data_[i]);
      ++size_;
    }
  }
  catch (...)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    ::operator delete(data_);
    throw;
  }
}

template< class T >
kuchukbaeva::Vector< T >& kuchukbaeva::Vector< T >::operator=(const Vector< T >& rhs)
{
  if (this != std::addressof(rhs))
  {
    Vector< T > copy(rhs);
    swap(copy);
  }
  return *this;
}

template< class T >
void kuchukbaeva::Vector< T >::swap(Vector< T >& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
}

template< class T >
size_t kuchukbaeva::Vector< T >::getSize() const noexcept
{
  return size_;
}

template< class T >
bool kuchukbaeva::Vector< T >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< class T >
size_t kuchukbaeva::Vector< T >::getCapacity() const noexcept
{
  return capacity_;
}

template< class T >
void kuchukbaeva::Vector< T >::reserve(size_t newCap)
{
  if (newCap <= capacity_)
  {
    return;
  }
  T* newData = static_cast< T* >(::operator new(newCap * sizeof(T)));
  size_t i = 0;
  try
  {
    for (; i < size_; ++i)
    {
      new (newData + i) T(data_[i]);
    }
  }
  catch (...)
  {
    for (size_t j = 0; j < i; ++j)
    {
      newData[j].~T();
    }
    ::operator delete(newData);
    throw;
  }

  for (size_t j = 0; j < size_; ++j)
  {
    data_[j].~T();
  }
  ::operator delete(data_);

  data_ = newData;
  capacity_ = newCap;
}

template< class T >
void kuchukbaeva::Vector< T >::pushBack(const T& value)
{
  if (size_ == capacity_)
  {
    T copy = value;
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    new (data_ + size_) T(std::move(copy));
  }
  else
  {
    new (data_ + size_) T(value);
  }
  ++size_;
}


template< class T >
void kuchukbaeva::Vector< T >::pushBack(T&& value)
{
  if (size_ == capacity_)
  {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
  }
  new (data_ + size_) T(std::move(value));
  ++size_;
}

template< class T >
void kuchukbaeva::Vector< T >::popBack()
{
  if (size_ == 0)
  {
    throw std::out_of_range("Vector is empty");
  }
  data_[size_ - 1].~T();
  --size_;
}

template< class T >
void kuchukbaeva::Vector< T >::insert(size_t id, const T& value)
{
  if (id > size_)
  {
    throw std::out_of_range("id out of bound");
  }
  Vector< T > copy;
  copy.reserve((size_ == capacity_) ? (capacity_ == 0 ? 1 : capacity_ * 2) : capacity_);
  for (size_t i = 0; i < id; ++i)
  {
    copy.pushBack(data_[i]);
  }
  copy.pushBack(value);
  for (size_t i = id; i < size_; ++i)
  {
    copy.pushBack(data_[i]);
  }
  swap(copy);
}

template< class T >
void kuchukbaeva::Vector< T >::erase(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("id out of bound");
  }
  Vector< T > copy;
  copy.reserve((size_ == capacity_) ? (capacity_ == 0 ? 1 : capacity_ * 2) : capacity_);
  for (size_t i = 0; i < id; ++i)
  {
    copy.pushBack(data_[i]);
  }
  for (size_t i = id + 1; i < size_; ++i)
  {
    copy.pushBack(data_[i]);
  }
  swap(copy);
}

template< class T >
T& kuchukbaeva::Vector< T >::at(size_t id)
{
  if (id >= size_)
  {
    throw std::out_of_range("id out of bound");
  }
  return data_[id];
}

template< class T >
const T& kuchukbaeva::Vector< T >::at(size_t id) const
{
  if (id >= size_)
  {
    throw std::out_of_range("id out of bound");
  }
  return data_[id];
}

template< class T >
kuchukbaeva::Viter< T > kuchukbaeva::Vector< T >::begin() noexcept
{
  return Viter< T >(data_);
}

template< class T >
kuchukbaeva::Viter< T > kuchukbaeva::Vector< T >::end() noexcept
{
  return Viter< T >(data_ + size_);
}

template< class T >
kuchukbaeva::Vciter< T > kuchukbaeva::Vector< T >::begin() const noexcept
{
  return Vciter< T >(data_);
}

template< class T >
kuchukbaeva::Vciter< T > kuchukbaeva::Vector< T >::end() const noexcept
{
  return Vciter< T >(data_ + size_);
}

template< class T >
kuchukbaeva::Vciter< T > kuchukbaeva::Vector< T >::cbegin() const noexcept
{
  return Vciter< T >(data_);
}

template< class T >
kuchukbaeva::Vciter< T > kuchukbaeva::Vector< T >::cend() const noexcept
{
  return Vciter< T >(data_ + size_);
}

#endif
