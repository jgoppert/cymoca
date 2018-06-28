//
// Created by jgoppert on 6/28/18.
//

#include "Component.h"

namespace cymoca {
namespace ast {

Component::Component(const string &name) : Node(typeid(*this)), _name(name) {
}

void Component::swapChild(Node *oldChild, unique_ptr<Node> newChild) {
}

vector<Node *> Component::children() const {
  return {};
}


unique_ptr<Node> Component::clone() const {
  return make_unique<Component>(name());
}

} // ast
} // cymoca