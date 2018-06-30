//
// Created by jgoppert on 6/28/18.
//

#include "SimpleStatement.h"
#include "ComponentRef.h"
#include "cymoca_compiler/util.h"

namespace cymoca {
namespace ast {

SimpleStatement::SimpleStatement(unique_ptr<ComponentRef> ref, unique_ptr<Expr> expr) :
    Statement(typeid(*this)), _ref(move(ref)), _expr(move(expr)) {
  _ref->parent(this);
  _expr->parent(this);
}

vector<Node *> SimpleStatement::children() const {
  return {_ref.get(), _expr.get()};
}

void SimpleStatement::swapChild(Node &oldChild, unique_ptr<Node> newChild) {
  if (&oldChild == _ref.get()) {
    auto e = static_unique_ptr_cast<ComponentRef>(move(newChild));
    _ref.swap(e);
    return;
  } else if (&oldChild == _expr.get()) {
    auto e = static_unique_ptr_cast<Expr>(move(newChild));
    _expr.swap(e);
    return;
  }
  assert(false);
}

unique_ptr<Node> SimpleStatement::clone() const {
  return make_unique<SimpleStatement>(
      _ref->cloneAs<ComponentRef>(),
      _expr->cloneAs<Expr>());
}

} // ast
} // cymoca