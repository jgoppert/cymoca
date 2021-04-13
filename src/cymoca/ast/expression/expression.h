#ifndef CYMOCA_AST_EXPRESSION_EXPRESSION_H_
#define CYMOCA_AST_EXPRESSION_EXPRESSION_H_

#include <string>
#include "../node.h"

namespace cymoca::ast::expression {

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
  class NAME : public TBinary<Base, Base, Base> {   \
   public:                                          \
    NODE_MACRO                                      \
    using TBinary::TBinary;                         \
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

/**
 * A list of expressions
 */
class List : public TList<Base, INode> {
 public:
  NODE_MACRO
  using TList::TList;
  std::unique_ptr<INode> clone() const override { return cloneList<List>(); }
};

/**
 * A function call.
 */
class Function : public Base {
 protected:
  std::unique_ptr<Reference> m_ref;
  std::unique_ptr<List> m_args;

 public:
  NODE_MACRO
  explicit Function(std::unique_ptr<Reference> ref, std::unique_ptr<List> args)
      : m_ref(std::move(ref)), m_args(std::move(args)) {}
  const Reference &getReference() const { return *m_ref; }
  List &getArgs() const { return *m_args; }
  std::vector<INode *> getChildren() override {
    return {m_ref.get(), m_args.get()};
  }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Function>(m_ref->cloneAs<Reference>(),
                                      m_args->cloneAs<List>());
  }
};

#define UNARY_EXPR_MACRO(NAME)                      \
  class NAME : public TUnary<Base, Base> {          \
   public:                                          \
    NODE_MACRO                                      \
    using TUnary::TUnary;                           \
    std::unique_ptr<INode> clone() const override { \
      return cloneUnary<NAME>();                    \
    }                                               \
  };

/**
 * The logical not operator, not a
 */
UNARY_EXPR_MACRO(Negative)

}  // namespace cymoca::ast::expression
#endif
