//
// Created by Joshua2018 on 2020/4/5.
//

#pragma once

#include "Map.h"
#include <stdexcept>
#include <iostream>

namespace ben_birdland {
template<typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Node::Node() {
    pair_ = nullptr;
    parent_ = nullptr;
    l_child_ = nullptr;
    r_child_ = nullptr;
}

template<typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Node::Node(const KeyType &key, const ValueType &value) {
    pair_ = new std::pair<KeyType, ValueType>(key, value);
    parent_ = nullptr;
    l_child_ = nullptr;
    r_child_ = nullptr;
}

template<typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::Node::operator==(const Map::Node &rhs) {
    return this->pair_->first == rhs.pair_->first;
}

template<typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::Node::operator!=(const Map::Node &rhs) {
    return this->pair_->first != rhs.pair_->first;
}

template<typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::Node::operator<(const Map::Node &rhs) {
    return this->pair_->first < rhs.pair_->first;
}

template<typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::Node::operator>(const Map::Node &rhs) {
    return this->pair_->first> rhs.pair_->first;
}

template<typename KeyType, typename ValueType>
Map<KeyType, ValueType>::iterator::iterator() {
    node_ = nullptr;
}
template<typename KeyType, typename ValueType>
Map<KeyType, ValueType>::iterator::iterator(Map::Node *node) {
    node_ = node;
}

template<typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::iterator &Map<KeyType, ValueType>::iterator::operator++() {
    if (node_ == nullptr) {
        throw std::runtime_error("Iterator is null");
    } else {
        Node *new_node_ = NextLocalNode(node_);
        if (new_node_ == nullptr) {
            new_node_ = node_;
            while (new_node_ != nullptr) {
                if (*(new_node_) > *(node_)) {
                    break;
                } else {
                    new_node_ = new_node_->parent_;
                }
            }
        }

        node_ = new_node_;
    }

    return *this;
}

template<typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::iterator &Map<KeyType, ValueType>::iterator::operator--() {
    if (node_ == nullptr) {
        throw std::runtime_error("Iterator is null");
    } else {
        Node *new_node_ = PrevLocalNode(node_);
        if (new_node_ == nullptr) {
            new_node_ = node_;
            while (new_node_ != nullptr) {
                if (*(new_node_) < *(node_)) {
                    break;
                } else {
                    new_node_ = new_node_->parent_;
                }
            }
        }

        node_ = new_node_;
    }

    return *this;
}

template<typename KeyType, typename ValueType>
std::pair<KeyType, ValueType> &Map<KeyType, ValueType>::iterator::operator*() const {
    return *(node_->pair_);
}

template<typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::iterator::operator!=(const Map::iterator &rhs) const {
    return this->node_ != rhs.node_;
}

template<typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::iterator Map<KeyType, ValueType>::begin() {
Node *current = root_;
while (current != nullptr) {
    if (current->l_child_ == nullptr) {
        break;
    } else {
        current = current->l_child_;
    }
}

return iterator(current);
}

template<typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::iterator Map<KeyType, ValueType>::end() {
    return Map::iterator();
}

template<typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map() {
    root_ = nullptr;
}

template<typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::Insert(const KeyType &key, const ValueType &value) {
    Node *current = root_, *parent = nullptr;
    while (current != nullptr) {
        if (key < current->pair_->first) {
            parent = current;
            current = current->l_child_;
        } else if (key > current->pair_->first) {
            parent = current;
            current = current->r_child_;
        } else {
            current->pair_->second = value;
            return;
        }
    }

    current = new Node(key, value);
    current->parent_ = parent;

    if (parent) {
        if (key < parent->pair_->first) {
            parent->l_child_ = current;
        } else if (key > parent->pair_->first) {
            parent->r_child_ = current;
        }
    } else {
        root_ = current;
    }
}

template<typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::Contains(const KeyType &key) const {
    return NodeAt(key) != nullptr;
}

template<typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::Remove(const KeyType &key) {
    Node *current = root_;
    while (current != nullptr) {
        if (key < current->pair_->first) {
            current = current->l_child_;
        } else if (key > current->pair_->first) {
            current = current->r_child_;
        } else {
            break;
        }
    }
    if (current == nullptr) {
        throw std::runtime_error("Key does not exist in the Map object");
    }

    if (current->l_child_ == nullptr and current->r_child_ == nullptr) {
        if (current->parent_ != nullptr) {
            if (*(current->parent_) < *(current)) {
                current->parent_->r_child_ = nullptr;
            } else {
                current->parent_->l_child_ = nullptr;
            }
        }
    } else if (current->l_child_ != nullptr and current->r_child_ != nullptr) {
        Node *new_current = NextLocalNode(current);

        if (new_current != nullptr) {
            current->pair_ = new_current->pair_;

            if (new_current == current->r_child_) {
                current->r_child_ = new_current->r_child_;
                if (current->r_child_ != nullptr) {
                    current->r_child_->parent_ = current;
                }
            } else {
                Node *parent = new_current->parent_;
                parent->l_child_ = new_current->r_child_;
                if (parent->l_child_ != nullptr) {
                    parent->l_child_->parent_ = parent;
                }
            }

            delete new_current;
        }
    } else if (current->l_child_ != nullptr) {
        current->pair_ = current->l_child_->pair_;

        delete current->l_child_;
        current->l_child_ = nullptr;
    } else {
        current->pair_ = current->r_child_->pair_;

        delete current->r_child_;
        current->r_child_ = nullptr;
    }
}

template<typename KeyType, typename ValueType>
ValueType &Map<KeyType, ValueType>::operator[](const KeyType &key) {
    Node *node = NodeAt(key);
    if (node != nullptr) {
        return node->pair_->second;
    } else {
        throw std::runtime_error("Key does not exist in the Map object");
    }
}

template<typename KeyType, typename ValueType>
const ValueType &Map<KeyType, ValueType>::operator[](const KeyType &key) const {
    Node *node = NodeAt(key);
    if (node != nullptr) {
        return node->pair_->second;
    } else {
        throw std::runtime_error("Key does not exist in the Map object");
    }
}

template<typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::Node *Map<KeyType, ValueType>::NodeAt(const KeyType &key) const {
    Node *current = root_;
    while (current != nullptr) {
        std::cout << "current = (" << current->pair_->first << ", " << current->pair_->second << ")\n";
        if (key < current->pair_->first) {
            current = current->l_child_;
        } else if (key > current->pair_->first) {
            current = current->r_child_;
        } else {
            break;
        }
    }
    return current;
}

template<typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::Node *Map<KeyType, ValueType>::PrevLocalNode(Map::Node *node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node *prev_node = nullptr;
    if (node->l_child_ != nullptr) {
        prev_node = node->l_child_;
        while (prev_node->r_child_ != nullptr) {
            prev_node = prev_node->r_child_;
        }
    } else if (node->parent_ != nullptr) {
        if (*(node->parent_) < *(node)) {
            prev_node = node->parent_;
        }
    }

    return prev_node;
}

template<typename KeyType, typename ValueType>
typename Map<KeyType, ValueType>::Node *Map<KeyType, ValueType>::NextLocalNode(Map::Node *node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node *next_node = nullptr;
    if (node->r_child_ != nullptr) {
        next_node = node->r_child_;
        while (next_node->l_child_ != nullptr) {
            next_node = next_node->l_child_;
        }
    } else if (node->parent_ != nullptr) {
        if (*(node->parent_) > *(node)) {
            next_node = node->parent_;
        }
    }

    return next_node;
}
}