//
// Created by Joshua2018 on 2020/4/5.
//

#ifndef HW_2__MAP_H_
#define HW_2__MAP_H_

#include <iterator>

namespace ben_birdland {
template <typename KeyType, typename ValueType>
class Map {
public:
    class Node {
    public:
        std::pair<KeyType, ValueType> *pair_;
        Node *parent_;
        Node *l_child_;
        Node *r_child_;

        Node();
        Node(const KeyType &key, const ValueType &value);

        bool operator==(const Node &rhs);
        bool operator!=(const Node &rhs);
        bool operator<(const Node &rhs);
        bool operator>(const Node &rhs);
    };

    class iterator : public std::iterator<std::bidirectional_iterator_tag, std::pair<KeyType, ValueType>> {
    public:
        iterator();
        explicit iterator(Node *node);
        iterator &operator++();
        iterator &operator--();
        std::pair<KeyType, ValueType> &operator*() const;
        bool operator!=(const iterator &rhs) const;
    private:
        Node *node_;
    };

    iterator begin();
    iterator end();

    Map();
    void Insert(const KeyType &key, const ValueType &value);
    bool Contains(const KeyType &key) const;
    void Remove(const KeyType &key);
    ValueType &operator[](const KeyType &key);
    const ValueType &operator[](const KeyType &key) const;

private:
    Node *NodeAt(const KeyType &key) const;
    static Node *PrevLocalNode(Node *node);
    static Node *NextLocalNode(Node *node);

    Node *root_;
};
}

#include "Map.hpp"

#endif //HW_2__MAP_H_
