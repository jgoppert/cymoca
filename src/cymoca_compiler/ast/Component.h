//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

namespace cymoca {
namespace ast {

class Component : public Node {
 public:
  NODE_MACRO(Component)
  explicit Component(const string &name);
  // accessors
  const std::string &name() const { return _name; };
  // node interface
  vector<Node *> children() const override;
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;
 protected:
  string _name;
};

} // ast
} // cymoca