//
// Created by jgoppert on 6/28/18.
//

#include "Component.h"
#include "ComponentRef.h"

namespace cymoca {
namespace ast {

Component::Component(const string &name, const string &type,
                     const Prefix &prefix)
    : Element(typeid(*this)), _name(name),
      _type(make_unique<ComponentRef>(type)), _prefix(prefix) {
  _type->parent(this);
}

void Component::swapChild(Node &oldChild, unique_ptr<Node> newChild) {
  if (&oldChild == _type.get()) {
    auto c = static_unique_ptr_cast<ComponentRef>(move(newChild));
    _type.swap(c);
  } else {
    assert(false);
  }
}

vector<Node *> Component::children() const { return {_type.get()}; }

unique_ptr<Node> Component::clone() const {
  return make_unique<Component>(name(), _type->name(), _prefix);
}

unordered_map<Prefix, string, EnumClassHash> prefixStr = {
    {Prefix::PARAMETER, "parameter"},
    {Prefix::CONSTANT, "constant"},
    {Prefix::VARIABLE, ""},
};

} // namespace ast
} // namespace cymoca

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
