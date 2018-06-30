//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca {
namespace ast {


class ComponentRef : public Expr {
 public:
  NODE_MACRO(ComponentRef)
  explicit ComponentRef(const string &name);
  // accessors
  const string &name() const { return _name; }
  // node interface
  vector<Node *> children() const override;
  void swapChild(Node &oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;
 protected:
  string _name;
};

} // ast
} // cymoca
