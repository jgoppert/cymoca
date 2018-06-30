//
// Created by jgoppert on 6/28/18.
//

#include "Node.h"
#include "Component.h"

namespace cymoca {
namespace ast {


class Class : public Node {
 public:
  NODE_MACRO(Class)
  Class(unique_ptr<Dict<Component>> components, unique_ptr<List<Equation>> equations);
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

} // ast
} // cymoca