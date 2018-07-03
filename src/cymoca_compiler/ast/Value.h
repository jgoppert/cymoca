//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca::ast {

template <class T, class Base> class Value : public Base {
public:
  NODE_MACRO(Value)
  explicit Value(T val) : Base(typeid(*this)), _val(val){};
  // accessors
  T &val() { return _val; }
  const T &val() const { return _val; }
  // node interface
  vector<Node *> children() const override { return {}; }
  void swapChild(Node &oldChild, unique_ptr<Node> newChild) override {}
  unique_ptr<Node> clone() const override { return make_unique<Value>(val()); }

  unique_ptr<Value<T>> cloneAsValue() const {
    return static_unique_ptr_cast<Value<T>>(clone());
  }

protected:
  T _val;
};

} // namespace cymoca::ast

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
