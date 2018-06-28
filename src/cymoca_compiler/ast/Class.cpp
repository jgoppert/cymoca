//
// Created by jgoppert on 6/28/18.
//

#include "Class.h"
#include "Dict.h"
#include "List.h"
#include "Component.h"

namespace cymoca {
namespace ast {

Class::Class(unique_ptr<Dict<Component>> components, unique_ptr<List<Equation>> equations) :
    Node(typeid(*this)), _components(move(components)), _equations(move(equations)) {
  _components->setParent(this);
  _equations->setParent(this);
}

Class::Class() : Class(make_unique<Dict<Component>>(), make_unique<List<Equation>>()) {

}

vector<Node *> Class::children() const {
  return {_components.get(), _equations.get()};
}

void Class::swapChild(Node *oldChild, unique_ptr<Node> newChild) {
  if (oldChild == _components.get()) {
    auto e = static_unique_ptr_cast<Dict<Component>>(move(newChild));
    _components.swap(e);
  } else if (oldChild == _equations.get()) {
    auto e = static_unique_ptr_cast<List<Equation>>(move(newChild));
    _equations.swap(e);
  }
}

unique_ptr<Node> Class::clone() const {
  auto comps = static_unique_ptr_cast<Dict<Component>>(_components->clone());
  auto eqs = static_unique_ptr_cast<List<Equation>>(_equations->clone());
  auto c = make_unique<Class>(move(comps), move(eqs));
  return move(c);
}

} // ast
} // cymoca