//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"

#include <unordered_map>

namespace cymoca {
namespace ast {

enum class Prefix {
  PARAMETER, CONSTANT, VARIABLE
};

class Component : public Node {
 public:
  NODE_MACRO(Component)
  Component(const string &name, const string & type, const Prefix &prefix);
  // accessors
  const std::string &name() const { return _name; };
  const Prefix & prefix() const { return _prefix; };
  // node interface
  vector<Node *> children() const override;
  void swapChild(Node *oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;
 protected:
  string _name;
  unique_ptr<ComponentRef> _type;
  Prefix _prefix;

};

extern unordered_map<Prefix, string, EnumClassHash> prefixStr;

} // ast
} // cymoca