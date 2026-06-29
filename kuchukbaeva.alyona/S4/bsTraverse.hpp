#ifndef BSTRAVERSE_HPP
#define BSTRAVERSE_HPP

#include "treeNode.hpp"

namespace kuchukbaeva
{
  template< class Key, class Value >
  TreeNode< Key, Value >* fallLeft(TreeNode< Key, Value >* node) noexcept;

  template< class Key, class Value >
  TreeNode< Key, Value >* fallRight(TreeNode< Key, Value >* node) noexcept;

  template< class Key, class Value >
  TreeNode< Key, Value >* next(TreeNode< Key, Value >* node) noexcept;

  template< class Key, class Value >
  TreeNode< Key, Value >* previous(TreeNode< Key, Value >* node) noexcept;
}

template< class Key, class Value >
kuchukbaeva::TreeNode< Key, Value >* kuchukbaeva::fallLeft(TreeNode< Key, Value >* node) noexcept
{
  if (!node)
  {
    return nullptr;
  }
  while (node->left_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value >
kuchukbaeva::TreeNode< Key, Value >* kuchukbaeva::fallRight(TreeNode< Key, Value >* node) noexcept
{
  if (!node)
  {
    return nullptr;
  }
  while (node->right_)
  {
    node = node->right_;
  }
  return node;
}

template< class Key, class Value >
kuchukbaeva::TreeNode< Key, Value >* kuchukbaeva::next(TreeNode< Key, Value >* node) noexcept
{
  if (!node)
  {
    return nullptr;
  }
  if (node->right_)
  {
    return fallLeft(node->right_);
  }
  TreeNode< Key, Value >* parent = node->parent_;
  while (parent && (parent->right_ == node))
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}

template< class Key, class Value >
kuchukbaeva::TreeNode< Key, Value >* kuchukbaeva::previous(TreeNode< Key, Value >* node) noexcept
{
  if (!node)
  {
    return nullptr;
  }
  if (node->left_)
  {
    return fallRight(node->left_);
  }
  TreeNode< Key, Value >* parent = node->parent_;
  while (parent && (parent->left_ == node))
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}

#endif
