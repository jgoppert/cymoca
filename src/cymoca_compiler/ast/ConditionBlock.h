//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca::ast {

template <class T> class ConditionBlock : public T {
public:
  NODE_MACRO(ConditionBlock)
  ConditionBlock(unique_ptr<LogicExpr> condition, unique_ptr<List<T>> list)
      : T(typeid(*this)), _condition(move(condition)), _list(move(list)) {}
  // accessors
  LogicExpr &condition() { return *_condition; }
  const LogicExpr &condition() const { return *_condition; }
  List<T> &list() { return *_list; }
  const List<T> &list() const { return *_list; }
  // node interface
  vector<Node *> children() const override {
    return {_condition.get(), _list.get()};
  }
  void swapChild(Node &oldChild, unique_ptr<Node> newChild) override {
    if (_condition.get() == &oldChild) {
      auto e = static_unique_ptr_cast<LogicExpr>(move(newChild));
      _condition.swap(e);
      return;
    } else if (_list.get() == &oldChild) {
      auto e = static_unique_ptr_cast<List<T>>(move(newChild));
      _list.swap(e);
      return;
    }
    assert(false);
  }
  unique_ptr<Node> clone() const override {
    auto c = _condition->cloneAs<LogicExpr>();
    auto l = _list->cloneAsList();
    auto cb = make_unique<ConditionBlock>(move(c), move(l));
    return move(cb);
  }

protected:
  unique_ptr<LogicExpr> _condition;
  unique_ptr<List<T>> _list;
};

} // namespace cymoca::ast

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
