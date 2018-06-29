//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"
#include "cymoca_compiler/util.h"

#include <unordered_map>

namespace cymoca {
namespace ast {

template<class Base, class Term, class Op>
class Unary : public Base {
 public:
  NODE_MACRO(Unary)
  Unary(Op op, unique_ptr<Term> right) :
      Base(typeid(*this)), _op(op), _right(move(right)) {
    _right->setParent(this);
  }
  // accessors
  Op op() const { return _op; }
  const Term &right() const { return *_right; }
  // node interface
  vector<Node *> children() const override {
    return {_right.get()};
  }
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override {
    if (oldChild == _right.get()) {
      auto e = static_unique_ptr_cast<Term>(move(newChild));
      _right.swap(e);
      return;
    }
    assert(false);
  }
  unique_ptr<Node> clone() const override {
    return make_unique<Unary>(
        op(), static_unique_ptr_cast<Term>(_right->clone()));
  }
 protected:
  Op _op;
  unique_ptr<Term> _right;
};

extern unordered_map<UnaryOp, string, EnumClassHash> unaryOpStr;
extern unordered_map<UnaryLogicOp, string, EnumClassHash> unaryLogicOpStr;

} // ast
} // cymoca

