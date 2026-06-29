#ifndef VECTORITER_HPP
#define VECTORITER_HPP
#include <iterator>

namespace kuchukbaeva
{
  template< class T > class Vector;
  template< class T > class Vciter;

  template< class T >
  struct Viter
  {
    T* p;
    Viter() noexcept;
    explicit Viter(T* ptr) noexcept;
    T& operator*() const;
    Viter< T >& operator++();
    Viter< T > operator++(int);
    Viter< T >& operator--();
    Viter< T > operator--(int);
    bool operator==(const Viter< T >& other) const;
    bool operator!=(const Viter< T >& other) const;
  };

  template< class T >
  struct Vciter
  {
    const T* p;
    Vciter() noexcept;
    explicit Vciter(const T* ptr) noexcept;
    Vciter(const Viter< T >& other);
    const T& operator*() const;
    Vciter< T >& operator++();
    Vciter< T > operator++(int);
    Vciter< T >& operator--();
    Vciter< T > operator--(int);
    bool operator==(const Vciter< T >& other) const;
    bool operator!=(const Vciter< T >& other) const;
  };
}

template< class T >
kuchukbaeva::Viter< T >::Viter() noexcept:
  p(nullptr)
{}

template< class T >
kuchukbaeva::Viter< T >::Viter(T* ptr) noexcept:
  p(ptr)
{}

template< class T >
T& kuchukbaeva::Viter< T >::operator*() const
{
  return *p;
}

template< class T >
kuchukbaeva::Viter< T >& kuchukbaeva::Viter< T >::operator++()
{
  ++p;
  return *this;
}

template< class T >
kuchukbaeva::Viter< T > kuchukbaeva::Viter< T >::operator++(int)
{
  Viter tmp = *this;
  ++p;
  return tmp;
}

template< class T >
kuchukbaeva::Viter< T >& kuchukbaeva::Viter< T >::operator--()
{
  --p;
  return *this;
}

template< class T >
kuchukbaeva::Viter< T > kuchukbaeva::Viter< T >::operator--(int)
{
  Viter tmp = *this;
  --p;
  return tmp;
}

template< class T >
bool kuchukbaeva::Viter< T >::operator==(const Viter< T >& other) const
{
  return p == other.p;
}

template< class T >
bool kuchukbaeva::Viter< T >::operator!=(const Viter< T >& other) const
{
  return p != other.p;
}

template< class T >
kuchukbaeva::Vciter< T >::Vciter() noexcept:
  p(nullptr)
{}

template< class T >
kuchukbaeva::Vciter< T >::Vciter(const T* ptr) noexcept:
  p(ptr)
{}

template< class T >
kuchukbaeva::Vciter< T >::Vciter(const Viter< T >& other):
  p(other.p)
{}

template< class T >
const T& kuchukbaeva::Vciter< T >::operator*() const
{
  return *p;
}

template< class T >
kuchukbaeva::Vciter< T >& kuchukbaeva::Vciter< T >::operator++()
{
  ++p;
  return *this;
}

template< class T >
kuchukbaeva::Vciter< T > kuchukbaeva::Vciter< T >::operator++(int)
{
  Vciter tmp = *this;
  ++p;
  return tmp;
}

template< class T >
kuchukbaeva::Vciter< T >& kuchukbaeva::Vciter< T >::operator--()
{
  --p;
  return *this;
}

template< class T >
kuchukbaeva::Vciter< T > kuchukbaeva::Vciter< T >::operator--(int)
{
  Vciter tmp = *this;
  --p;
  return tmp;
}

template< class T >
bool kuchukbaeva::Vciter< T >::operator==(const Vciter< T >& other) const
{
  return p == other.p;
}

template< class T >
bool kuchukbaeva::Vciter< T >::operator!=(const Vciter< T >& other) const
{
  return p != other.p;
}

#endif
