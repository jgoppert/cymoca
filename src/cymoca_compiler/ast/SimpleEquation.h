//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca::ast {

class SimpleEquation : public Equation {
public:
  NODE_MACRO(SimpleEquation)
  SimpleEquation(unique_ptr<Expr> left, unique_ptr<Expr> right);
  // accessors
  const Expr &left() const { return *_left; }
  const Expr &right() const { return *_right; }
  Expr &left() { return *_left; }
  Expr &right() { return *_right; }
  vector<Node *> children() const override;
  void swapChild(Node &oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;

protected:
  unique_ptr<Expr> _left;
  unique_ptr<Expr> _right;
};

} // namespace cymoca::ast

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
