#ifndef TREE_ITERATORS_HPP
#define TREE_ITERATORS_HPP
#include <iterator>
#include "tree-node.hpp"

namespace alymova
{
  template< class Key, class Value, class Comparator >
  class TwoThreeTree;
  template< class Key, class Value, class Comparator >
  struct TTTIterator;

  template< class Key, class Value, class Comparator >
  struct TTTConstIterator:
    public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
  {
  public:
    using ConstIterator = TTTConstIterator< Key, Value, Comparator >;
    using Node = typename detail::TTTNode< Key, Value >;
    using NodeType = typename detail::TTTNode< Key, Value >::NodeType;
    using NodePoint = typename detail::NodePoint;

    TTTConstIterator();
    ConstIterator& operator++() noexcept;
    ConstIterator& operator++(int) noexcept;
    ConstIterator& operator--() noexcept;
    ConstIterator& operator--(int) noexcept;
    bool operator==(const ConstIterator& other) const noexcept;
    bool operator!=(const ConstIterator& other) const noexcept;
    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >* operator->() const noexcept;
  private:
    const Node* node_;
    NodePoint point_;

    TTTConstIterator(Node* node, NodePoint point);
    TTTConstIterator(TTTIterator< Key, Value, Comparator > it);

    friend class TwoThreeTree< Key, Value, Comparator >;
    friend class TTTIterator< Key, Value, Comparator >;
  };

  template< class Key, class Value, class Comparator >
  struct TTTIterator:
    public std::iterator< std::bidirectional_iterator_tag, std::pair< Key, Value > >
  {
  public:
    using Iterator = TTTIterator< Key, Value, Comparator >;
    using Node = typename detail::TTTNode< Key, Value >;
    using NodeType = typename detail::TTTNode< Key, Value >::NodeType;
    using NodePoint = typename detail::NodePoint;

    TTTIterator();
    Iterator& operator++() noexcept;
    Iterator& operator++(int) noexcept;
    Iterator& operator--() noexcept;
    Iterator& operator--(int) noexcept;
    bool operator==(const Iterator& other) const noexcept;
    bool operator!=(const Iterator& other) const noexcept;
    std::pair< Key, Value >& operator*() noexcept;
    std::pair< Key, Value >* operator->() noexcept;
  private:
    Node* node_;
    NodePoint point_;

    TTTIterator(TTTConstIterator< Key, Value, Comparator > it);
    TTTIterator(Node* node, NodePoint point);

    friend class TwoThreeTree< Key, Value, Comparator >;
    friend class TTTConstIterator< Key, Value, Comparator >;
  };
}
#endif
