//
// Created by jgoppert on 6/28/18.
//

#include "ComponentRef.h"

namespace cymoca {
namespace ast {

ComponentRef::ComponentRef(const string &name)
    : Expr(typeid(*this)), _name(name) {}

vector<Node *> ComponentRef::children() const { return {}; }

void ComponentRef::swapChild(Node &oldChild, unique_ptr<Node> newChild) {}

unique_ptr<Node> ComponentRef::clone() const {
  return make_unique<ComponentRef>(name());
}

} // namespace ast
} // namespace cymoca

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
