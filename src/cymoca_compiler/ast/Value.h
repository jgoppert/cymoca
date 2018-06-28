//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca {
namespace ast {

template<class T, class Base>
class Value : public Base {
 public:
  NODE_MACRO(Value)
  explicit Value(T val) : Base(typeid(*this)), _val(val) {};
  // accessors
  T val() const { return _val; }
  // node interface
  vector<Node *> children() const override { return {}; }
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override {}
  unique_ptr<Node> clone() const override {
    return make_unique<Value>(val());
  }
 protected:
  T _val;
};

} // ast
} // cymoca


