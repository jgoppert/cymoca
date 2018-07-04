//
// Created by jgoppert on 6/28/18.
//

#include "FunctionCall.h"

namespace cymoca::ast {

FunctionCall::FunctionCall(const string &name, unique_ptr<Args> args)
    : Expr(typeid(*this)), _name(name), _args(move(args)) {
  _args->parent(this);
}

FunctionCall::FunctionCall(const string &name, unique_ptr<Expr> ref)
    : Expr(typeid(*this)), _name(name), _args(make_unique<Args>()) {
  _args->parent(this);
  _args->append(move(ref));
}

void FunctionCall::swapChild(Node &oldChild, unique_ptr<Node> newChild) {
  if (_args.get() == &oldChild) {
    auto newArgs = static_unique_ptr_cast<Args>(move(newChild));
    _args.swap(newArgs);
  }
}

vector<Node *> FunctionCall::children() const { return {_args.get()}; }

unique_ptr<Node> FunctionCall::clone() const {
  return make_unique<FunctionCall>(name(), _args->cloneAsList());
}

} // namespace cymoca::ast

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
