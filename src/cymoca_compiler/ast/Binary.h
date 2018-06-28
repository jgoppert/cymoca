//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"
#include "cymoca_compiler/util.h"

#include <map>

namespace cymoca {
namespace ast {

template<class Base, class Term, class Op>
class Binary : public Base {
 public:
  NODE_MACRO(Binary)
  Binary(std::unique_ptr<Term> left, Op op, std::unique_ptr<Term> right) :
      Base(typeid(*this)), _left(move(left)), _op(op), _right(move(right)) {
    _left->setParent(this);
    _right->setParent(this);
  }
  // accessors
  const Term &left() const { return *_left; }
  const Term &right() const { return *_right; }
  Op op() const { return _op; }
  // node interface

  vector<Node *> children() const override {
    return {_left.get(), _right.get()};
  }
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override {
    if (oldChild == _left.get()) {
      auto e = static_unique_ptr_cast<Term>(move(newChild));
      _left.swap(e);
    } else if (oldChild == _right.get()) {
      auto e = static_unique_ptr_cast<Term>(move(newChild));
      _right.swap(e);
    } else {
      assert(false);
    }
  }
  unique_ptr<Node> clone() const override {
    return make_unique<Binary>(
        static_unique_ptr_cast<Term>(_left->clone()),
        op(),
        static_unique_ptr_cast<Term>(_right->clone()));
  }
 protected:
  std::unique_ptr<Term> _left;
  Op _op;
  std::unique_ptr<Term> _right;
};

extern map<BinaryOp, string> binaryOpStr;
extern map<UnaryOp, string> unaryOpStr;
extern map<RelationOp, string> relationOpStr;
extern map<BinaryLogicOp, string> binaryLogicOpStr;

} // ast
} // cymoca