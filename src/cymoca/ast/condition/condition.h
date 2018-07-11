#ifndef CYMOCA_AST_CONDITION_CONDITION_H_
#define CYMOCA_AST_CONDITION_CONDITION_H_

#include "../node.h"

namespace cymoca::ast::condition {

#define RELATION_MACRO(NAME)                                              \
  class NAME : public TBinary<expression::Base, expression::Base, Base> { \
    NODE_MACRO                                                            \
    using TBinary::TBinary;                                               \
    std::unique_ptr<INode> clone() const override {                       \
      return cloneBinary<NAME>();                                         \
    }                                                                     \
  };

/**
 * The less than operator, a < b
 */
RELATION_MACRO(LessThan)

/**
 * The less than or equal to operator, a <= b
 */
RELATION_MACRO(LessThanOrEqual)

/**
 * The greater than operator, a > b
 */
RELATION_MACRO(GreaterThan)

/**
 * The greater than or equal to operator, a >= b
 */
RELATION_MACRO(GreaterThanOrEqual)

/**
 * The equal operator, a == b
 */
RELATION_MACRO(Equal)

/**
 * The not equal operator, a <> b
 */
RELATION_MACRO(NotEqual)

#define BINARY_COND_MACRO(NAME)                     \
  class NAME : public TBinary<Base, Base, Base> {   \
   public:                                          \
    NODE_MACRO                                      \
    using TBinary::TBinary;                         \
    std::unique_ptr<INode> clone() const override { \
      return cloneBinary<NAME>();                   \
    }                                               \
  };

/**
 * The logical and operator, a and b
 */
BINARY_COND_MACRO(And)

/**
 * The logical or operator, a or b
 */
BINARY_COND_MACRO(Or)

#define UNARY_COND_MACRO(NAME)                      \
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
UNARY_COND_MACRO(Not)

/**
 * A constant boolean condition, true or false
 */
class Boolean : public Base {
 protected:
  bool m_value;

 public:
  NODE_MACRO
  explicit Boolean(bool value) : m_value(value) {}
  bool getValue() const { return m_value; }
  void setValue(bool value) { m_value = value; }
  std::vector<INode *> getChildren() override { return {}; }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Boolean>(m_value);
  }
};

}  // namespace cymoca::ast::condition

#endif
