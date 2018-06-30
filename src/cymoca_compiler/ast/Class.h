//
// Created by jgoppert on 6/28/18.
//

#include "Component.h"
#include "Node.h"

namespace cymoca {
namespace ast {

class Class : public Node {
public:
  NODE_MACRO(Class)
  Class(unique_ptr<Dict<Component>> components,
        unique_ptr<List<Equation>> equations);
  Class();
  // accessors
  const Dict<Component> &components() const { return *_components; };
  const List<Equation> &equations() const { return *_equations; };
  Dict<Component> &components() { return *_components; };
  List<Equation> &equations() { return *_equations; };
  // node interface
  vector<Node *> children() const override;
  void swapChild(Node &oldChild, unique_ptr<Node> newChild) override;
  unique_ptr<Node> clone() const override;

protected:
  unique_ptr<Dict<Component>> _components;
  unique_ptr<List<Equation>> _equations;
};

} // namespace ast
} // namespace cymoca

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
