//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"
#include "cymoca_compiler/util.h"

#include <unordered_map>

namespace cymoca {
namespace ast {

template<class T>
class Dict : public Node {
 public:
  NODE_MACRO(Dict)
  Dict() : Node(typeid(*this)), _map() {}
  Dict(initializer_list<pair<string, unique_ptr<T>>> pairs) :
  Node(typeid(*this)), _map() {
    for (auto &p: pairs) {
      set(p.first, static_unique_ptr_cast<T>(p.second->clone()));
    }
  }
  // accessors
  void set(const string &name, unique_ptr<T> val) {
    val->parent(this);
    _map[name] = move(val);
  }
  T &get(const string &name) {
    T &v = *_map[name];
    assert(&v);
    return v;
  }
  unordered_map<string, unique_ptr<T>> & memory() { return _map; }

  // node interface
  vector<Node *> children() const override {
    vector<Node *> v;
    for (auto &c: _map) {
      v.push_back(c.second.get());
    }
    return v;
  }
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override {
    // TODO: doing a linear search to find the right pointer, slow
    for (auto &c: _map) {
      if (c.second.get() == oldChild) {
        auto e = static_unique_ptr_cast<T>(move(newChild));
        c.second.swap(e);
        return;
      }
    }
    assert(false);
  }
  unique_ptr<Node> clone() const override {
    auto newDict = make_unique<Dict>();
    for (auto &c: _map) {
      auto e = static_unique_ptr_cast<T>(move(c.second->clone()));
      newDict->set(c.first, move(e));
    }
    return move(newDict);
  }

  unique_ptr<Dict<T>> cloneAsDict() {
    return static_unique_ptr_cast<Dict<T>>(clone());
  }

 protected:
  unordered_map<string, unique_ptr<T>> _map;
};

} // ast
} // cymoca