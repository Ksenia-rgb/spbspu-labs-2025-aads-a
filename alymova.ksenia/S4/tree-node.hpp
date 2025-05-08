#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP
#include <utility>
#include <iostream>

namespace alymova
{
  namespace detail
  {
    enum NodePoint {Empty, First, Second};

    template< class Key, class Value >
    struct TTTNode
    {
      using Node = TTTNode< Key, Value >;

      std::pair< Key, Value > data[3];
      enum NodeType {Empty, Double, Triple, Overflow} type;
      Node* parent;
      Node* left;
      Node* mid;
      Node* right;
      Node* overflow;

      TTTNode();
      TTTNode(const std::pair< Key, Value >& pair);
      TTTNode(const std::pair< Key, Value >& pair, Node* parent, Node* left, Node* mid, Node* right, Node* overflow);

      void insert(const std::pair< Key, Value >& value);
      bool isLeaf();
    };

    template< class Key, class Value >
    TTTNode< Key, Value >::TTTNode():
      data{std::pair< Key, Value >(), std::pair< Key, Value >()},
      type(NodeType::Empty),
      parent(nullptr),
      left(nullptr),
      mid(nullptr),
      right(nullptr),
      overflow(nullptr)
    {}

    template< class Key, class Value >
    TTTNode< Key, Value >::TTTNode(const std::pair< Key, Value>& pair):
      data{pair, std::pair< Key, Value >()},
      type(NodeType::Double),
      parent(nullptr),
      left(nullptr),
      mid(nullptr),
      right(nullptr),
      overflow(nullptr)
    {}

    template< class Key, class Value >
    TTTNode< Key, Value >::TTTNode(const std::pair< Key, Value >& pair,
      Node* parent, Node* left, Node* mid, Node* right, Node* overflow):
      data{pair, std::pair< Key, Value >()},
      type(NodeType::Double),
      parent(parent),
      left(left),
      mid(mid),
      right(right),
      overflow(overflow)
    {}

    template< class Key, class Value >
    void TTTNode< Key, Value >::insert(const std::pair< Key, Value >& value)
    {
      data[type] = value;
      if (type == NodeType::Empty)
      {
        type = NodeType::Double;
      }
      else if (type == NodeType::Double)
      {
        if (data[1].first < data[0].first)
        {
          std::swap(data[0], data[1]);
        }
        type = NodeType::Triple;
      }
      else if (type == NodeType::Triple)
      {
        if (data[2].first < data[1].first)
        {
          std::swap(data[2], data[1]);
        }
        if (data[1].first < data[0].first)
        {
          std::swap(data[1], data[0]);
        }
        type = NodeType::Overflow;
      }
    }

    template< class Key, class Value >
    bool TTTNode< Key, Value >::isLeaf()
    {
      if (right)
      {
        if (right->type != NodeType::Empty)
        {
          return false;
        }
      }
      return (!left && !mid && !overflow);
    }
  }
}

#endif
