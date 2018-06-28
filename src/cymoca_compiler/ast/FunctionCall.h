//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"
#include "List.h"

namespace cymoca {
namespace ast {

class FunctionCall : public Expr {
 public:
  NODE_MACRO(FunctionCall)
  FunctionCall(const string &name, unique_ptr<Args> args);
  // accessors
  const std::string &name() const { return _name; };
  const Args & args() const { return *_args; };
  Args & args() { return *_args; };
  // node interface
  vector<Node *> children() const override;
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;
 protected:
  string _name;
  unique_ptr<Args> _args;
};

} // ast
} // cymoca