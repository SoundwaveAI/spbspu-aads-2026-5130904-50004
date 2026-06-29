#ifndef BSTREEITER_HPP
#define BSTREEITER_HPP

#include "treeNode.hpp"
#include "bsTraverse.hpp"

namespace kuchukbaeva
{
  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    using Node = TreeNode< Key, Value >;

    BSTIterator() noexcept;
    explicit BSTIterator(Node* node) noexcept;

    std::pair< const Key, Value >& operator*() const noexcept;
    std::pair< const Key, Value >* operator->() const noexcept;

    BSTIterator& operator++() noexcept;
    BSTIterator operator++(int) noexcept;
    BSTIterator& operator--() noexcept;
    BSTIterator operator--(int) noexcept;

    bool operator==(const BSTIterator& other) const noexcept;
    bool operator!=(const BSTIterator& other) const noexcept;

  private:
    Node* node_;
    template< class K, class V, class C > friend class BSTree;
    friend class BSTConstIterator< Key, Value >;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    using Node = TreeNode< Key, Value >;

    BSTConstIterator() noexcept;
    explicit BSTConstIterator(Node* node) noexcept;
    BSTConstIterator(const BSTIterator< Key, Value >& it) noexcept;

    const std::pair< const Key, Value >& operator*() const noexcept;
    const std::pair< const Key, Value >* operator->() const noexcept;

    BSTConstIterator& operator++() noexcept;
    BSTConstIterator operator++(int) noexcept;
    BSTConstIterator& operator--() noexcept;
    BSTConstIterator operator--(int) noexcept;

    bool operator==(const BSTConstIterator& other) const noexcept;
    bool operator!=(const BSTConstIterator& other) const noexcept;

  private:
    Node* node_;
    template< class K, class V, class C > friend class BSTree;
  };
}

template< class Key, class Value >
kuchukbaeva::BSTIterator< Key, Value >::BSTIterator() noexcept:
  node_(nullptr)
{}

template< class Key, class Value >
kuchukbaeva::BSTIterator< Key, Value >::BSTIterator(Node* node) noexcept:
  node_(node)
{}

template< class Key, class Value >
std::pair< const Key, Value >& kuchukbaeva::BSTIterator< Key, Value >::operator*() const noexcept
{
  return node_->data_;
}

template< class Key, class Value >
std::pair< const Key, Value >* kuchukbaeva::BSTIterator< Key, Value >::operator->() const noexcept
{
  return &(node_->data_);
}

template< class Key, class Value >
kuchukbaeva::BSTIterator< Key, Value >& kuchukbaeva::BSTIterator< Key, Value >::operator++() noexcept
{
  node_ = next(node_);
  return *this;
}

template< class Key, class Value >
kuchukbaeva::BSTIterator< Key, Value > kuchukbaeva::BSTIterator< Key, Value >::operator++(int) noexcept
{
  BSTIterator tmp(*this);
  ++(*this);
  return tmp;
}

template< class Key, class Value >
kuchukbaeva::BSTIterator< Key, Value >& kuchukbaeva::BSTIterator< Key, Value >::operator--() noexcept
{
  node_ = previous(node_);
  return *this;
}

template< class Key, class Value >
kuchukbaeva::BSTIterator< Key, Value > kuchukbaeva::BSTIterator< Key, Value >::operator--(int) noexcept
{
  BSTIterator tmp(*this);
  --(*this);
  return tmp;
}

template< class Key, class Value >
bool kuchukbaeva::BSTIterator< Key, Value >::operator==(const BSTIterator& other) const noexcept
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool kuchukbaeva::BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const noexcept
{
  return !(*this == other);
}

template< class Key, class Value >
kuchukbaeva::BSTConstIterator< Key, Value >::BSTConstIterator() noexcept:
  node_(nullptr)
{}

template< class Key, class Value >
kuchukbaeva::BSTConstIterator< Key, Value >::BSTConstIterator(Node* node) noexcept:
  node_(node)
{}

template< class Key, class Value >
kuchukbaeva::BSTConstIterator< Key, Value >::BSTConstIterator(const BSTIterator< Key, Value >& it) noexcept:
  node_(it.node_)
{}

template< class Key, class Value >
const std::pair< const Key, Value >& kuchukbaeva::BSTConstIterator< Key, Value >::operator*() const noexcept
{
  return node_->data_;
}

template< class Key, class Value >
const std::pair< const Key, Value >* kuchukbaeva::BSTConstIterator< Key, Value >::operator->() const noexcept
{
  return &(node_->data_);
}

template< class Key, class Value >
kuchukbaeva::BSTConstIterator< Key, Value >& kuchukbaeva::BSTConstIterator< Key, Value >::operator++() noexcept
{
  node_ = next(node_);
  return *this;
}

template< class Key, class Value >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTConstIterator< Key, Value >::operator++(int) noexcept
{
  BSTConstIterator tmp(*this);
  ++(*this);
  return tmp;
}

template< class Key, class Value >
kuchukbaeva::BSTConstIterator< Key, Value >& kuchukbaeva::BSTConstIterator< Key, Value >::operator--() noexcept
{
  node_ = previous(node_);
  return *this;
}

template< class Key, class Value >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTConstIterator< Key, Value >::operator--(int) noexcept
{
  BSTConstIterator tmp(*this);
  --(*this);
  return tmp;
}

template< class Key, class Value>
bool kuchukbaeva::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator& other) const noexcept
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool kuchukbaeva::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator& other) const noexcept
{
  return !(*this == other);
}

#endif
