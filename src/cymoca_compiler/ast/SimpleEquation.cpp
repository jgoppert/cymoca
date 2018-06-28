//
// Created by jgoppert on 6/28/18.
//

#include "SimpleEquation.h"
#include "cymoca_compiler/util.h"

namespace cymoca {
namespace ast {

SimpleEquation::SimpleEquation(unique_ptr<Expr> left, unique_ptr<Expr> right)
    : Equation(typeid(*this)), _left(move(left)), _right(move(right)) {
  _left->setParent(this);
  _right->setParent(this);
}

vector<Node *> SimpleEquation::children() const {
  return {_left.get(), _right.get()};
}

void SimpleEquation::swapChild(Node *oldChild, unique_ptr<Node> newChild) {
  if (oldChild == _left.get()) {
    auto e = static_unique_ptr_cast<Expr>(move(newChild));
    _left.swap(e);
    return;
  } else if (oldChild == _right.get()) {
    auto e = static_unique_ptr_cast<Expr>(move(newChild));
    _right.swap(e);
    return;
  }
  assert(false);
}

unique_ptr<Node> SimpleEquation::clone() const {
  return make_unique<SimpleEquation>(
      static_unique_ptr_cast<Expr>(_left->clone()),
      static_unique_ptr_cast<Expr>(_right->clone()));
}

} // ast
} // cymoca
