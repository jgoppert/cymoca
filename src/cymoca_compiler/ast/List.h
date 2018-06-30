//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"
#include "cymoca_compiler/util.h"

namespace cymoca {
namespace ast {

template<class T, class Base, class Label>
class List : public Base {
 public:
  NODE_MACRO(List)
  List(initializer_list<unique_ptr<T>> vals) : Base(typeid(*this)), _list() {
    // TODO no way to avoid cloning here?
    for (auto &v: vals) {
      auto c = static_unique_ptr_cast<T>(v->clone());
      append(move(c));
    }
  }
  List() : Base(typeid(*this)), _list() {
  }
  // accessors
  // node interface
  void append(unique_ptr<T> val) {
    val->parent(this);
    _list.push_back(move(val));
  }
  vector<T *> elements() {
    vector<T *> v;
    for (auto & e: _list) {
      v.push_back(e.get());
    }
    return v;
  }
  vector<const T *> elements() const {
    vector<const T *> v;
    for (auto & e: _list) {
      v.push_back(e.get());
    }
    return v;
  }
  vector<unique_ptr<T>> & memory() {
    return _list;
  }
  vector<Node *> children() const override {
    vector<Node *> v;
    for (auto &c: _list) {
      v.push_back(c.get());
    }
    return v;
  }
  void swapChild(Node &oldChild, unique_ptr<Node> newChild) override {
    // TODO: doing a linear search to find the right pointer, slow
    for (auto &c: _list) {
      if (c.get() == &oldChild) {
        auto e = static_unique_ptr_cast<T>(move(newChild));
        c.swap(e);
        return;
      }
    }
    assert(false);
  }
  unique_ptr<Node> clone() const override {
    auto l = make_unique<List>();
    for (auto &c: _list) {
      auto e = static_unique_ptr_cast<T>(move(c->clone()));
      l->append(move(e));
    }
    return move(l);
  }

  unique_ptr<List<T>> cloneAsList() {
    return static_unique_ptr_cast<List<T>>(clone());
  }

 protected:
  vector<unique_ptr<T>> _list;
};

} // ast
} // cymoca

