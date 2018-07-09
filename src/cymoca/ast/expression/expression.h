#ifndef CYMOCA_AST_EXPRESSION_EXPRESSION_H_
#define CYMOCA_AST_EXPRESSION_EXPRESSION_H_

#include <string>
#include "../node.h"

namespace cymoca::ast::expression {

/**
 * The base expression class.
 */
class Base : virtual public INode {};

/**
 * A reference to a component.
 */
class Reference : public Base {
 protected:
  std::string m_name;

 public:
  NODE_MACRO
  virtual ~Reference() = default;
  explicit Reference(std::string name) : m_name(std::move(name)) {}
  const std::string &getName() const { return m_name; }
  void setName(const std::string &name) { m_name = name; }
  std::vector<INode *> getChildren() override { return {}; }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Reference>(getName());
  }
};

#define BINARY_EXPR_MACRO(NAME)                     \
  class NAME : public Binary<Base, Base, Base> {    \
   public:                                          \
    NODE_MACRO                                      \
    using Binary::Binary;                           \
    std::unique_ptr<INode> clone() const override { \
      return cloneBinary<NAME>();                   \
    }                                               \
  };

/**
 * Add two expressions.
 */
BINARY_EXPR_MACRO(Add)

/**
 * Subtract two expressions.
 */
BINARY_EXPR_MACRO(Subtract)

/**
 * Multiply two expressions.
 */
BINARY_EXPR_MACRO(Multiply)

/**
 * Divide two expressions.
 */
BINARY_EXPR_MACRO(Divide)

/**
 * A constant number expression.
 */
class Number : public Base {
 protected:
  double _value;

 public:
  NODE_MACRO
  explicit Number(double value) : _value(value) {}
  double getValue() const { return _value; }
  void setValue(double value) { _value = value; }
  std::vector<INode *> getChildren() override { return {}; }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Number>(getValue());
  }
};

}  // namespace cymoca::ast::expression
#endif
