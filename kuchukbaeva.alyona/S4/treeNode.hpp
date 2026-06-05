#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <utility>

namespace kuchukbaeva
{
  template< class Key, class Value >
  struct TreeNode
  {
    TreeNode();
    TreeNode(const Key& key, const Value& value, TreeNode* parent = nullptr);
    std::pair< const Key, Value > data_;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode* parent_;
  };
}

template< class Key, class Value >
kuchukbaeva::TreeNode< Key, Value >::TreeNode():
  data_{},
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}

template< class Key, class Value >
kuchukbaeva::TreeNode< Key, Value >::TreeNode(const Key& key, const Value& value, TreeNode* parent):
  data_(key, value),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

#endif
