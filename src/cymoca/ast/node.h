#ifndef CYMOCA_AST_NODE_H_
#define CYMOCA_AST_NODE_H_

#include <cassert>
#include <vector>

namespace cymoca::ast {

/**
 * Forward declare all node templates used here.
 */

template <class Label>
class Choice;

template <class Item>
struct ConditionBlock;

template <class Choice>
struct Option;

template <class Element, class Base, class Label>
struct ConditionBlocks;

template <class Type, class Base>
struct Value;

template <class Left, class Right, class Base, class Label>
struct Binary;

template <class Right, class Base, class Label>
struct Unary;

/**
 * Forward declare every Node that will be listened to here.
 *
 * For nodes that are based on templates this suffices for their complete
 * declaration.
 */

/**
 * Choices: These are the node types used for type checking of generic
 * expressions in an AST.
 */
using Equation = Choice<struct LabelEquation>;
using Expression = Choice<struct LabelExpression>;
using Condition = Choice<struct LabelCondition>;
using Statement = Choice<struct LabelStatement>;

/**
 * Top level structures
 */
struct Node;
struct Walker;

/**
 * Expression: These are low level mathematical expressions, symbols,
 * function calls etc.
 */
namespace expression {
struct Function;
struct Reference;
using Number = Value<double, Option<Expression>>;
using Add = Binary<Expression, Expression, Option<Expression>, struct LabelAdd>;
using Subtract =
    Binary<Expression, Expression, Option<Expression>, struct LabelSubtract>;
using Multiply =
    Binary<Expression, Expression, Option<Expression>, struct LabelMultiply>;
using Divide =
    Binary<Expression, Expression, Option<Expression>, struct LabelDivide>;
using Negative = Unary<Expression, Option<Expression>, struct LabelNegative>;
}  // namespace expression

/**
 * Equation: Equations consist of a set of expressions that are related
 * through equality.
 */
namespace equation {
using Simple =
    Binary<Expression, Expression, Option<Equation>, struct LabelSimple>;
using Block = ConditionBlock<Equation>;
using If = ConditionBlocks<Equation, Option<Equation>, struct LabelIf>;
using When = ConditionBlocks<Equation, Option<Equation>, struct LabelWhen>;
}  // namespace equation

/**
 * Condition: Conditions are logical expressions
 * that evaluate to true or false.
 */
namespace condition {
using Bool = Value<bool, Option<Condition>>;
using Less =
    Binary<Expression, Expression, Option<Condition>, struct LabelLess>;
using LessOrEqual =
    Binary<Expression, Expression, Option<Condition>, struct LabelLessOrEqual>;
using Greater =
    Binary<Expression, Expression, Option<Condition>, struct LabelGreater>;
using GreaterOrEqual = Binary<Expression, Expression, Option<Condition>,
                              struct LabelGreaterOrEqual>;
using Equal =
    Binary<Expression, Expression, Option<Condition>, struct LabelEqual>;
using NotEqual =
    Binary<Expression, Expression, Option<Condition>, struct LabelNotEqual>;
using And = Binary<Condition, Condition, Option<Condition>, struct LabelAnd>;
using Or = Binary<Condition, Condition, Option<Condition>, struct LabelOr>;
using Not = Unary<Condition, Option<Condition>, struct LabelNot>;

}  // namespace condition

/**
 * Model: Models are high level tree components like classes.
 */
namespace model {
struct Class;
}  // namespace model

/**
 * Use this macro to declare new nodes to listen to.
 */
#define LISTEN(NAME)            \
  virtual void enter(NAME &){}; \
  virtual void exit(NAME &){};

/**
 * @brief The Listener struct
 * If you add a new node, use the LISTEN macro to
 * add the required hooks to listener and use
 * the NODE macro in the node function. This listener
 * impelments all callbacks by default with empty bodies.
 * Derive from this class and override the methods desired.
 */
struct Listener {
  virtual ~Listener() = default;
  virtual void enter_every(Node &){};
  virtual void exit_every(Node &){};
  LISTEN(expression::Add)
  LISTEN(expression::Subtract)
  LISTEN(expression::Multiply)
  LISTEN(expression::Divide)
  LISTEN(expression::Number)
  LISTEN(expression::Reference)
  LISTEN(expression::Function)
  LISTEN(expression::Negative)
  LISTEN(equation::If)
  LISTEN(equation::When)
  LISTEN(equation::Simple)
  LISTEN(condition::Bool)
  LISTEN(condition::And)
  LISTEN(condition::Or)
  LISTEN(condition::Not)
  LISTEN(condition::Less)
  LISTEN(condition::LessOrEqual)
  LISTEN(condition::Greater)
  LISTEN(condition::GreaterOrEqual)
  LISTEN(condition::Equal)
  LISTEN(condition::NotEqual)
  LISTEN(model::Class)
  LISTEN(equation::Block)
};

/**
 * @brief The Node interface
 * This pure virtual interface defines methods used for listening to and
 * walking the tree.
 */
struct Node {
  virtual ~Node() = default;
  virtual void enter(Listener &) = 0;
  virtual void exit(Listener &) = 0;
  virtual std::vector<Node *> children() = 0;
};

/**
 * Use this macro to implement the listener methods for a node.
 */
#define NODE                                            \
  void enter(Listener &l) override { l.enter(*this); }; \
  void exit(Listener &l) override { l.exit(*this); };

/**
 * A simple tree walker.
 */
struct Walker {
  void walk(Node &node, Listener &listener) {
    listener.enter_every(node);
    node.enter(listener);
    for (auto c : node.children()) {
      assert(c);
      walk(*c, listener);
    }
    node.exit(listener);
    listener.exit_every(node);
  }
};

}  // namespace cymoca::ast

#endif
