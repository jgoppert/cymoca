//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "List.h"
#include "Node.h"

namespace cymoca {
namespace ast {

class FunctionCall : public Expr {
public:
  NODE_MACRO(FunctionCall)
  FunctionCall(const string &name, unique_ptr<Args> args);
  /**
   * Convenience ctor for single argument.
   * @param name name of function
   * @param expr single expression
   */
  FunctionCall(const string &name, unique_ptr<Expr> expr);
  // accessors
  const string &name() const { return _name; };
  const Args &args() const { return *_args; };
  Args &args() { return *_args; };
  // node interface
  vector<Node *> children() const override;
  void swapChild(Node &oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;

protected:
  string _name;
  unique_ptr<Args> _args;
};

} // namespace ast
} // namespace cymoca

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
