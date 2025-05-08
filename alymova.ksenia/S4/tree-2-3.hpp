#ifndef TREE_2_3_HPP
#define TREE_2_3_HPP
#include <cstddef>
#include <cassert>
#include <functional>
#include <iostream>
#include "tree-iterators.hpp"
#include "tree-iterator-impl.hpp"
#include "tree-const-iterator-impl.hpp"

namespace alymova
{
  template< class Key, class Value, class Comparator >
  class TwoThreeTree
  {
  public:
    using Tree = TwoThreeTree< Key, Value, Comparator >;
    using Iterator = TTTIterator< Key, Value, Comparator >;
    using ConstIterator = TTTConstIterator< Key, Value, Comparator >;
    using Node = typename detail::TTTNode< Key, Value >;
    using NodeType = typename detail::TTTNode< Key, Value >::NodeType;
    using NodePoint = typename detail::NodePoint;
    using T = std::pair< Key, Value >;

    TwoThreeTree();
    TwoThreeTree(const Tree& other);
    TwoThreeTree(Tree&& other) noexcept;
    template< typename InputIterator>
    TwoThreeTree(InputIterator first, InputIterator last);
    TwoThreeTree(std::initializer_list< T > il);
    ~TwoThreeTree() noexcept;

    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    Iterator begin();
    ConstIterator begin() const noexcept;
    ConstIterator cbegin() const noexcept;
    Iterator end();
    ConstIterator end() const noexcept;
    ConstIterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    std::pair< Iterator, bool > insert(const T& value);
    std::pair< Iterator, bool > insert(T&& value);
    template < class InputIterator >
    void insert(InputIterator first, InputIterator last);
    void insert(std::initializer_list< T > il);
    Iterator insert(Iterator hint, const T& value);
    Iterator insert(ConstIterator hint, const T& value);
    template< class... Args >
    std::pair< Iterator, bool > emplace(Args&&... args);
    template< class... Args >
    Iterator emplace_hint(ConstIterator hint, Args&&... args);
    void swap(Tree& other);
    void clear() noexcept;

    size_t count(const Key& key) const;
    Iterator find(const Key& key);
    ConstIterator find(const Key& key) const;
    std::pair< Iterator, Iterator > equal_range(const Key& key);
    std::pair< ConstIterator, ConstIterator > equal_range(const Key& key) const;
    Iterator lower_bound(const Key& key);
    ConstIterator lower_bound(const Key& key) const;
    Iterator upper_bound(const Key& key);
    ConstIterator upper_bound(const Key& key) const;

  private:
    Node* fake_;
    Node* root_;
    size_t size_;
    Comparator cmp;

    void clear(Node* node) noexcept;
    void split(Node* node);
    void move_fake() noexcept;
    Node* find_to_insert(const Key& key) const;
    Node* find_to_insert(ConstIterator hint) const;
    bool check_hint(ConstIterator hint, const Key& key);
    bool is_balanced();
    size_t find_height(Node* node);
  };

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree():
    fake_(new Node{}),
    root_(fake_),
    size_(0)
  {}

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(const Tree& other):
    Tree(other.begin(), other.end())
  {}

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(Tree&& other) noexcept:
    fake_(std::exchange(other.fake_, nullptr)),
    root_(std::exchange(other.root_, nullptr)),
    size_(std::exchange(other.size_, 0))
  {}

  template< class Key, class Value, class Comparator >
  template< class InputIterator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(InputIterator first, InputIterator last):
    Tree()
  {
    for (auto it = first; it != last; it++)
    {
      emplace(*it);
    }
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::TwoThreeTree(std::initializer_list< T > il):
    Tree(il.begin(), il.end())
  {}

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >::~TwoThreeTree() noexcept
  {
    if (size_)
    {
      clear(root_);
    }
    delete fake_;
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >& TwoThreeTree< Key, Value, Comparator >::operator=(const Tree& other)
  {
    assert(this != std::addressof(other) && "Assigning a map to itself");
    Tree copy(other);
    swap(copy);
    return *this;
  }

  template< class Key, class Value, class Comparator >
  TwoThreeTree< Key, Value, Comparator >& TwoThreeTree< Key, Value, Comparator >::operator=(Tree&& other) noexcept
  {
    assert(this != std::addressof(other) && "Assigning a map to itself");
    Tree moved(std::move(other));
    swap(moved);
    return *this;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::begin()
  {
    return Iterator(cbegin());
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::begin() const noexcept
  {
    return cbegin();
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::cbegin() const noexcept
  {
    if (root_ == fake_)
    {
      return cend();
    }
    Node* tmp = root_;
    while (tmp->left)
    {
      tmp = tmp->left;
    }
    return ConstIterator(tmp, NodePoint::First);
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::end()
  {
    return Iterator(cend());
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::end() const noexcept
  {
    return cend();
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::cend() const noexcept
  {
    return ConstIterator(fake_, NodePoint::Empty);
  }

  template< class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Comparator >
  bool TwoThreeTree< Key, Value, Comparator >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTIterator< Key, Value, Comparator >, bool >
    TwoThreeTree< Key, Value, Comparator >::insert(const T& value)
  {
    return emplace(value);
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTIterator< Key, Value, Comparator >, bool >
    TwoThreeTree< Key, Value, Comparator >::insert(T&& value)
  {
    return emplace(std::forward< T >(value));
  }

  template< class Key, class Value, class Comparator >
  template< class InputIterator >
  void TwoThreeTree< Key, Value, Comparator >::insert(InputIterator first, InputIterator last)
  {
    for (auto it = first; it != last; it++)
    {
      insert(*it);
    }
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::insert(std::initializer_list< T > il)
  {
    insert(il.begin(), il.end());
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::insert(Iterator hint, const T& value)
  {
    ConstIterator const_hint(hint);
    return emplace_hint(const_hint, value);
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::insert(ConstIterator hint, const T& value)
  {
    return emplace_hint(hint, value);
  }

  template< class Key, class Value, class Comparator >
  template <class... Args >
  std::pair< TTTIterator< Key, Value, Comparator >, bool >
    TwoThreeTree< Key, Value, Comparator >::emplace(Args&&... args)
  {
    std::pair< Key, Value > value(std::forward< Args >(args)...);
    size_t size_before = size_;
    ConstIterator hint = lower_bound(value.first);
    Iterator it = emplace_hint(hint, value);
    if (size_before == size())
    {
      return {it, false};
    }
    return {it, true};
  }

  template< class Key, class Value, class Comparator >
  template <class... Args >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::emplace_hint(ConstIterator hint, Args&&... args)
  {
    std::pair< Key, Value > value(std::forward< Args >(args)...);
    auto it_value = find(value.first);
    if (it_value != end())
    {
      return it_value;
    }
    Node* to_insert = find_to_insert(hint);
    if (!check_hint(hint, value.first))
    {
      to_insert = find_to_insert(value.first);
    }
    if (size_ == 0)
    {
      root_ = new Node();
      to_insert = root_;
    }
    to_insert->insert(value);
    split(to_insert);
    move_fake();
    size_++;
    return find(value.first);
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::swap(Tree& other)
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::clear() noexcept
  {
    clear(root_);
  }

  template< class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::count(const Key& key) const
  {
    if (find(key) != end())
    {
      return 1;
    }
    return 0;
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::find(const Key& key)
  {
    ConstIterator tmp = static_cast< const Tree& >(*this).find(key);
    return Iterator(tmp);
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator > TwoThreeTree< Key, Value, Comparator >::find(const Key& key) const
  {
    for (ConstIterator it = cbegin(); it != cend(); it++)
    {
      if (!cmp(key, it->first) && !cmp(it->first, key))
      {
        return it;
      }
    }
    return cend();
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTIterator< Key, Value, Comparator >, TTTIterator< Key, Value, Comparator > >
    TwoThreeTree< Key, Value, Comparator >::equal_range(const Key& key)
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Comparator >
  std::pair< TTTConstIterator< Key, Value, Comparator >, TTTConstIterator< Key, Value, Comparator > >
    TwoThreeTree< Key, Value, Comparator >::equal_range(const Key& key) const
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::lower_bound(const Key& key)
  {
    ConstIterator tmp = static_cast< const Tree& >(*this).lower_bound(key);
    return Iterator(tmp);
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::lower_bound(const Key& key) const
  {
    for (auto it = cbegin(); it != cend(); it++)
    {
      if (!cmp(it->first, key))
      {
        return it;
      }
    }
    return cend();
  }

  template< class Key, class Value, class Comparator >
  TTTIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::upper_bound(const Key& key)
  {
    ConstIterator tmp = static_cast< const Tree& >(*this).upper_bound(key);
    return Iterator(tmp);
  }

  template< class Key, class Value, class Comparator >
  TTTConstIterator< Key, Value, Comparator >
    TwoThreeTree< Key, Value, Comparator >::upper_bound(const Key& key) const
  {
    for (auto it = cbegin(); it != cend(); it++)
    {
      if (cmp(key, it->first))
      {
        return it;
      }
    }
    return cend();
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::clear(Node* root) noexcept
  {
    if (!root || root == fake_)
    {
      return;
    }
    clear(root->left);
    clear(root->mid);
    clear(root->right);
    delete root;
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::split(Node* node)
  {
    if (node->type != NodeType::Overflow)
    {
      return;
    }
    Node* left_node = nullptr, *right_node = nullptr, *parent_node = nullptr;
    try
    {
      parent_node = node->parent;
      left_node = new Node(node->data[0], parent_node, node->left, nullptr, node->mid, nullptr);
      right_node = new Node(node->data[2], parent_node, node->right, nullptr, node->overflow, nullptr);
      if (left_node->left)
      {
        left_node->left->parent = left_node;
      }
      if (left_node->right)
      {
        left_node->right->parent = left_node;
      }
      if (right_node->left)
      {
        right_node->left->parent = right_node;
        if (right_node->left == fake_)
        {
          right_node->left = nullptr;
        }
      }
      if (right_node->right)
      {
        right_node->right->parent = right_node;
      }
      if (!parent_node)
      {
        parent_node = new Node(node->data[1], nullptr, left_node, nullptr, right_node, nullptr);
        left_node->parent = parent_node;
        right_node->parent = parent_node;
        root_ = parent_node;
        delete node;
        return;
      }
      parent_node->insert(node->data[1]);
      {
        if (parent_node->right == node && parent_node->type == NodeType::Triple)
        {
          parent_node->mid = left_node;
          parent_node->right = right_node;
        }
        else if (parent_node->right == node && parent_node->type == NodeType::Overflow)
        {
          parent_node->right = left_node;
          parent_node->overflow = right_node;
        }
        else if (parent_node->mid == node)
        {
          parent_node->mid = left_node;
          parent_node->right = right_node;
        }
        else if (parent_node->left == node)
        {
          parent_node->left = left_node;
          parent_node->mid = right_node;
        }
      }
    }
    catch (...)
    {
      delete left_node;
      delete right_node;
      delete parent_node;
      throw;
    }
    delete node;
    split(parent_node);
  }

  template< class Key, class Value, class Comparator >
  void TwoThreeTree< Key, Value, Comparator >::move_fake() noexcept
  {
    Node* tmp = root_;
    while (tmp->right && tmp->right != fake_)
    {
      tmp = tmp->right;
    }
    tmp->right = fake_;
    fake_->parent = tmp;
  }

  template< class Key, class Value, class Comparator >
  detail::TTTNode< Key, Value >* TwoThreeTree< Key, Value, Comparator >::find_to_insert(const Key& key) const
  {
    Node* tmp = root_;
    while (!tmp->isLeaf())
    {
      if (cmp(key, tmp->data[0].first))
      {
        tmp = tmp->left;
      }
      else if (tmp->type == NodeType::Triple && cmp(key, tmp->data[1].first))
      {
        tmp = tmp->mid;
      }
      else
      {
        tmp = tmp->right;
      }
    }
    return tmp;
  }

  template< class Key, class Value, class Comparator >
  detail::TTTNode< Key, Value >* TwoThreeTree< Key, Value, Comparator >::find_to_insert(ConstIterator hint) const
  {
    Iterator tmp = hint;
    if (hint == cbegin())
    {
      return root_;
    }
    return (--tmp).node_;
  }

  template< class Key, class Value, class Comparator >
  bool TwoThreeTree< Key, Value, Comparator >::check_hint(ConstIterator hint, const Key& key)
  {
    if (hint != cend())
    {
      if (!cmp(key, hint->first))
      {
        return false;
      }
    }
    if (hint != cbegin())
    {
      return cmp((--hint)->first, key);
    }
    return true;
  }

  template< class Key, class Value, class Comparator >
  bool TwoThreeTree< Key, Value, Comparator >::is_balanced()
  {
    return find_height(root_) != -1;
  }

  template< class Key, class Value, class Comparator >
  size_t TwoThreeTree< Key, Value, Comparator >::find_height(Node* node)
  {
    if (node == nullptr || node == fake_)
    {
      return 0;
    }
    size_t left_height = find_height(node->left);
    size_t right_height = find_height(node->right);
    size_t mid_height = find_height(node->mid);
    if (left_height < 0 || right_height < 0 || mid_height < 0)
    {
      return -1;
    }
    if (left_height != right_height)
    {
      return -1;
    }
    if (left_height != mid_height && node->type == NodeType::Triple)
    {
      return -1;
    }
    return left_height + 1;
  }
}

#endif
