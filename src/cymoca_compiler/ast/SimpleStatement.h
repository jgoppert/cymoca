//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca {
namespace ast {

class SimpleStatement : public Statement {
 public:
  NODE_MACRO(SimpleStatement)
  SimpleStatement(unique_ptr<ComponentRef> ref, unique_ptr<Expr> expr);
  // accessors
  const ComponentRef &ref() const { return *_ref; }
  const Expr & expr() const { return *_expr; }
  vector<Node *> children() const override;
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;
 protected:
  unique_ptr<ComponentRef> _ref;
  unique_ptr<Expr> _expr;
};

} // ast
} // cymoca

