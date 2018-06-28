//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca {
namespace ast {

template<class T>
class ConditionBlock : public T {
 public:
  NODE_MACRO(ConditionBlock)
  ConditionBlock(unique_ptr<LogicExpr> condition, unique_ptr<List<T>> list) :
      T(typeid(*this)), _condition(move(condition)), _list(move(list)) {}
  // accessors
  // node interface
  vector<Node *> children() const override {
    return {_condition.get(), _list.get()};
  }
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override {
    if (_condition.get() == oldChild) {
      auto e = static_unique_ptr_cast<LogicExpr>(move(newChild));
      _condition.swap(e);
      return;
    } else if (_list.get() == oldChild) {
      auto e = static_unique_ptr_cast<List<T>>(move(newChild));
      _list.swap(e);
      return;
    }
    assert(false);
  }
  unique_ptr<Node> clone() const override {
    auto condCopy = _condition->clone();
    auto c = static_unique_ptr_cast<LogicExpr>(move(condCopy));
    auto listCopy = _list->clone();
    auto l = static_unique_ptr_cast<List<T>>(move(listCopy));
    auto cb = make_unique<ConditionBlock>(move(c), move(l));
    return move(cb);
  }
 protected:
  unique_ptr<LogicExpr> _condition;
  unique_ptr<List<T>> _list;
};

} // ast
} // cymoca