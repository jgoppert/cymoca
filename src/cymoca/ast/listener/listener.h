#ifndef CYMOCA_AST_LISTENER_LISTENER_H_
#define CYMOCA_AST_LISTENER_LISTENER_H_

/**
 * Forward declare any class that can be listened to
 */

namespace cymoca::ast {

class INode;

namespace model {
class Class;
class ElementDict;
}  // namespace model

namespace expression {
class Add;
class Divide;
class Multiply;
class Number;
class Reference;
class Subtract;
}  // namespace expression

namespace condition {
class And;
class Boolean;
class Equal;
class GreaterThan;
class GreaterThanOrEqual;
class LessThan;
class LessThanOrEqual;
class Not;
class Or;
}  // namespace condition

namespace equation {
class Block;
class If;
class List;
class Simple;
class When;
}  // namespace equation

namespace statement {
class Block;
class If;
class When;
}  // namespace statement

namespace element {
class Component;
}  // namespace element

}  // namespace cymoca::ast

namespace cymoca::ast::listener {

#define LISTENER_MACRO(NAME)    \
  virtual void enter(NAME &) {} \
  virtual void exit(NAME &) {}

class Base {
 public:
  virtual ~Base(){};
  virtual void enterEvery(INode &) {}
  virtual void exitEvery(INode &) {}
  LISTENER_MACRO(condition::And)
  LISTENER_MACRO(condition::Boolean)
  LISTENER_MACRO(condition::Equal)
  LISTENER_MACRO(condition::GreaterThan)
  LISTENER_MACRO(condition::GreaterThanOrEqual)
  LISTENER_MACRO(condition::LessThan)
  LISTENER_MACRO(condition::LessThanOrEqual)
  LISTENER_MACRO(condition::Not)
  LISTENER_MACRO(condition::Or)
  LISTENER_MACRO(element::Component)
  LISTENER_MACRO(equation::Block)
  LISTENER_MACRO(equation::If)
  LISTENER_MACRO(equation::List)
  LISTENER_MACRO(equation::Simple)
  LISTENER_MACRO(equation::When)
  LISTENER_MACRO(expression::Add)
  LISTENER_MACRO(expression::Divide)
  LISTENER_MACRO(expression::Multiply)
  LISTENER_MACRO(expression::Number)
  LISTENER_MACRO(expression::Reference)
  LISTENER_MACRO(expression::Subtract)
  LISTENER_MACRO(model::Class)
  LISTENER_MACRO(model::ElementDict)
  LISTENER_MACRO(statement::Block)
  LISTENER_MACRO(statement::If)
  LISTENER_MACRO(statement::When)
};

}  // namespace cymoca::ast::listener

#endif
