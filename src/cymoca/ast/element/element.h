#ifndef CYMOCA_AST_ELEMENT_ELEMENT_H_
#define CYMOCA_AST_ELEMENT_ELEMENT_H_

#include <string>
#include "../expression/expression.h"
#include "../node.h"

namespace cymoca::ast::element {

enum class Prefix { VARIABLE, CONSTANT, PARAMETER, DISCRETE };

class Base : public INode {};

class Component : public Base {
 protected:
  std::string m_name;
  std::unique_ptr<expression::Reference> m_type;
  Prefix m_prefix;

 public:
  NODE_MACRO
  Component(std::string name, std::string type, Prefix prefix)
      : m_name(move(name)),
        m_type(std::make_unique<expression::Reference>(type)),
        m_prefix(prefix) {}
  const std::string &getName() const { return m_name; }
  const expression::Reference &getType() const { return *m_type; }
  const Prefix &getPrefix() const { return m_prefix; }
  std::vector<INode *> getChildren() override { return {m_type.get()}; }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Component>(getName(), getType().getName(),
                                       getPrefix());
  }
};

}  // namespace cymoca::ast::element

#endif
