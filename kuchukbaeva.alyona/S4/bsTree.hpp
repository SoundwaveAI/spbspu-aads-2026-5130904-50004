#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <functional>
#include "treeNode.hpp"
#include "bsTraverse.hpp"
#include "bsTreeIter.hpp"

namespace kuchukbaeva
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using Node = TreeNode< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    ~BSTree();

    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void push(const Key& k, const Value& v);
    Value& get(const Key& k);
    const Value& get(const Key& k) const;
    Value drop(const Key& k);

    size_t height(const_iterator it) const;
    size_t height() const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    iterator find(const Key& k);
    const_iterator find(const Key& k) const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    void swap(BSTree& other) noexcept;
    void clear();

  private:
    Node* root_;
    size_t size_;
    Compare cmp_;
    Node* findNode(const Key& k) const;
    Node* copyNodes(Node* node, Node* parent);
    size_t heightNode(const Node* node) const;
    void removeNode(Node* node);
    void clearNodes(Node* node);
  };
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTree< Key, Value, Compare >::BSTree():
  root_(nullptr),
  size_(0)
{}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  root_(copyNodes(other.root_, nullptr)),
  size_(other.size_),
  cmp_(other.cmp_)
{}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  BSTree()
{
  swap(other);
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTree< Key, Value, Compare >&
  kuchukbaeva::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  if (this != std::addressof(other))
  {
    BSTree tmp(other);
    swap(tmp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTree< Key, Value, Compare >&
  kuchukbaeva::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  if (this != std::addressof(other))
  {
    BSTree tmp(std::move(other));
    swap(tmp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
void kuchukbaeva::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  if (!root_)
  {
    root_ = new Node(k, v);
    ++size_;
    return;
  }

  Node* cur = root_;
  Node* parent = nullptr;

  while (cur)
  {
    parent = cur;
    if (cmp_(k, cur->data_.first))
    {
      cur = cur->left_;
    }
    else if (cmp_(cur->data_.first, k))
    {
      cur = cur->right_;
    }
    else
    {
      cur->data_.second = v;
      return;
    }
  }

  Node* newNode = new Node(k, v, parent);
  if (cmp_(k, parent->data_.first))
  {
    parent->left_ = newNode;
  }
  else
  {
    parent->right_ = newNode;
  }
  ++size_;
}

template< class Key, class Value, class Compare >
Value& kuchukbaeva::BSTree< Key, Value, Compare >::get(const Key& k)
{
  Node* node = findNode(k);
  if (!node)
  {
    throw std::out_of_range("Key not found");
  }
  return node->data_.second;
}

template< class Key, class Value, class Compare >
const Value& kuchukbaeva::BSTree< Key, Value, Compare >::get(const Key& k) const
{
  Node* node = findNode(k);
  if (!node)
  {
    throw std::out_of_range("Key not found");
  }
  return node->data_.second;
}

template< class Key, class Value, class Compare >
Value kuchukbaeva::BSTree< Key, Value, Compare >::drop(const Key& k)
{
  Node* node = findNode(k);
  if (!node)
  {
    throw std::out_of_range("Key not found");
  }

  Value dropped = node->data_.second;
  removeNode(node);
  --size_;
  return dropped;
}

template< class Key, class Value, class Compare >
size_t kuchukbaeva::BSTree< Key, Value, Compare >::height(const_iterator it) const
{
  return heightNode(it.node_);
}

template< class Key, class Value, class Compare >
size_t kuchukbaeva::BSTree< Key, Value, Compare >::height() const
{
  return heightNode(root_);
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
{
  Node* cur = it.node_;
  if (!cur)
  {
    throw std::logic_error("Invalid iterator");
  }
  Node* parent = cur->parent_;
  if (!parent || parent->right_ != cur)
  {
    throw std::logic_error("Cannot rotate left: node is not a right child");
  }

  Node* leftChild = cur->left_;
  Node* grand = parent->parent_;

  if (grand == nullptr)
  {
    root_ = cur;
  }
  else if (grand->left_ == parent)
  {
    grand->left_ = cur;
  }
  else
  {
    grand->right_ = cur;
  }
  cur->parent_ = grand;

  cur->left_ = parent;
  parent->parent_ = cur;
  parent->right_ = leftChild;

  if (leftChild)
  {
    leftChild->parent_ = parent;
  }
  return const_iterator(cur);
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
{
  Node* cur = it.node_;
  if (!cur)
  {
    throw std::logic_error("Invalid iterator");
  }
  Node* parent = cur->parent_;
  if (!parent || parent->left_ != cur)
  {
    throw std::logic_error("Cannot rotate right: node is not a left child");
  }

  Node* rightChild = cur->right_;
  Node* grand = parent->parent_;

  if (grand == nullptr)
  {
    root_ = cur;
  }
  else if (grand->left_ == parent)
  {
    grand->left_ = cur;
  }
  else
  {
    grand->right_ = cur;
  }

  cur->parent_ = grand;
  cur->right_ = parent;
  parent->parent_ = cur;
  parent->left_ = rightChild;

  if (rightChild)
  {
    rightChild->parent_ = parent;
  }
  return const_iterator(cur);
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value >
kuchukbaeva::BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
{
  Node* node = it.node_;
  if (!node)
  {
    throw std::logic_error("Invalid iterator");
  }
  Node* parent = node->parent_;
  if (!parent || parent->left_ != node)
  {
    throw std::logic_error("Cannot rotate large left: node must be left child of right child");
  }
  Node* grand = parent->parent_;
  if (!grand || grand->right_ != parent)
  {
    throw std::logic_error("Cannot rotate large left: parent must be right child");
  }
  rotateRight(const_iterator(node));
  return rotateLeft(const_iterator(node));
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value >
kuchukbaeva::BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
{
  Node* node = it.node_;
  if (!node)
  {
    throw std::logic_error("Invalid iterator");
  }
  Node* parent = node->parent_;
  if (!parent || parent->right_ != node)
  {
    throw std::logic_error("Cannot rotate large right: node must be right child of left child");
  }
  Node* grand = parent->parent_;
  if (!grand || grand->left_ != parent)
  {
    throw std::logic_error("Cannot rotate large right: parent must be left child");
  }
  rotateLeft(const_iterator(node));
  return rotateRight(const_iterator(node));
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::find(const Key& k)
{
  return iterator(findNode(k));
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::find(const Key& k) const
{
  return const_iterator(findNode(k));
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::begin() noexcept
{
  return iterator(kuchukbaeva::fallLeft(root_));
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::begin() const noexcept
{
  return const_iterator(kuchukbaeva::fallLeft(root_));
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  return begin();
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::end() noexcept
{
  return iterator(nullptr);
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::end() const noexcept
{
  return const_iterator(nullptr);
}

template< class Key, class Value, class Compare >
kuchukbaeva::BSTConstIterator< Key, Value > kuchukbaeva::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return end();
}

template< class Key, class Value, class Compare >
bool kuchukbaeva::BSTree< Key, Value, Compare >::isEmpty() const noexcept
{
  return root_ == nullptr;
}

template< class Key, class Value, class Compare >
size_t kuchukbaeva::BSTree< Key, Value, Compare >::getSize() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
void kuchukbaeva::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(cmp_, other.cmp_);
}

template< class Key, class Value, class Compare >
void kuchukbaeva::BSTree< Key, Value, Compare >::clear()
{
  clearNodes(root_);
  root_ = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
kuchukbaeva::TreeNode< Key, Value >* kuchukbaeva::BSTree< Key, Value, Compare >::findNode(const Key& k) const
{
  Node* cur = root_;
  while (cur)
  {
    if (cmp_(k, cur->data_.first))
    {
      cur = cur->left_;
    }
    else if (cmp_(cur->data_.first, k))
    {
      cur = cur->right_;
    }
    else
    {
      return cur;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
kuchukbaeva::TreeNode< Key, Value >* kuchukbaeva::BSTree< Key, Value, Compare >::copyNodes(Node* node, Node* parent)
{
  if (!node)
  {
    return nullptr;
  }

  Node* newNode = new Node(node->data_.first, node->data_.second, parent);
  try
  {
    newNode->left_ = copyNodes(node->left_, newNode);
    newNode->right_ = copyNodes(node->right_, newNode);
  }
  catch (...)
  {
    clearNodes(newNode);
    throw;
  }
  return newNode;
}

template< class Key, class Value, class Compare >
size_t kuchukbaeva::BSTree< Key, Value, Compare >::heightNode(const Node* node) const
{
  if (!node)
  {
    return 0;
  }
  size_t leftH = heightNode(node->left_);
  size_t rightH = heightNode(node->right_);
  return 1 + (leftH > rightH ? leftH : rightH);
}

template< class Key, class Value, class Compare >
void kuchukbaeva::BSTree< Key, Value, Compare >::removeNode(Node* node)
{
  if (!node)
  {
    return;
  }
  if (!node->left_ && !node->right_)
  {
    if (node == root_)
    {
      root_ = nullptr;
    }
    else if (node == node->parent_->left_)
    {
      node->parent_->left_ = nullptr;
    }
    else
    {
      node->parent_->right_ = nullptr;
    }
    delete node;
  }
  else if (!node->left_ || !node->right_)
  {
    Node* child = (node->left_ ? node->left_ : node->right_);
    child->parent_ = node->parent_;
    if (node == root_)
    {
      root_ = child;
    }
    else if (node == node->parent_->left_)
    {
      node->parent_->left_ = child;
    }
    else
    {
      node->parent_->right_ = child;
    }
    delete node;
  }
  else
  {
    Node* succ = fallLeft(node->right_);
    if (succ->parent_ == node)
    {
      succ->parent_ = node->parent_;
      if (node == root_)
      {
        root_ = succ;
      }
      else if (node == node->parent_->left_)
      {
        node->parent_->left_ = succ;
      }
      else
      {
        node->parent_->right_ = succ;
      }
      succ->left_ = node->left_;

      if (succ->left_)
      {
        succ->left_->parent_ = succ;
      }
    }
    else
    {
      succ->parent_->left_ = succ->right_;
      if (succ->right_)
      {
        succ->right_->parent_ = succ->parent_;
      }
      succ->right_ = node->right_;
      if (succ->right_)
      {
        succ->right_->parent_ = succ;
      }

      succ->left_ = node->left_;
      if (succ->left_)
      {
        succ->left_->parent_ = succ;
      }

      succ->parent_ = node->parent_;
      if (node == root_)
      {
        root_ = succ;
      }
      else if (node == node->parent_->left_)
      {
        node->parent_->left_ = succ;
      }
      else
      {
        node->parent_->right_ = succ;
      }
    }
    delete node;
  }
}

template< class Key, class Value, class Compare >
void kuchukbaeva::BSTree< Key, Value, Compare >::clearNodes(Node* node)
{
  if (!node)
  {
    return;
  }
  clearNodes(node->left_);
  clearNodes(node->right_);
  delete node;
}

#endif
