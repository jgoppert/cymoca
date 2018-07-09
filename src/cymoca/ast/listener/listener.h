#ifndef CYMOCA_AST_LISTENER_LISTENER_H_
#define CYMOCA_AST_LISTENER_LISTENER_H_

#include "../declarations.h"

namespace cymoca::ast::listener {

#define LISTENER_MACRO(NAME)    \
  virtual void enter(NAME &) {} \
  virtual void exit(NAME &) {}

class Base {
 public:
  virtual ~Base(){};
  virtual void enterEvery(INode &) {}
  virtual void exitEvery(INode &) {}
  LISTENER_MACRO(Class)
  LISTENER_MACRO(ElementDict)
  LISTENER_MACRO(expression::Add)
  LISTENER_MACRO(expression::Subtract)
  LISTENER_MACRO(expression::Multiply)
  LISTENER_MACRO(expression::Divide)
  LISTENER_MACRO(expression::Reference)
  LISTENER_MACRO(expression::Number)
  LISTENER_MACRO(condition::Boolean)
  LISTENER_MACRO(condition::And)
  LISTENER_MACRO(condition::Or)
  LISTENER_MACRO(condition::Not)
  LISTENER_MACRO(condition::LessThan)
  LISTENER_MACRO(condition::LessThanOrEqual)
  LISTENER_MACRO(condition::GreaterThan)
  LISTENER_MACRO(condition::GreaterThanOrEqual)
  LISTENER_MACRO(condition::Equal)
  LISTENER_MACRO(equation::Block)
  LISTENER_MACRO(equation::Simple)
  LISTENER_MACRO(equation::If)
  LISTENER_MACRO(equation::When)
  LISTENER_MACRO(statement::Block)
  LISTENER_MACRO(statement::If)
  LISTENER_MACRO(statement::When)
  LISTENER_MACRO(element::Component)
};

}  // namespace cymoca::ast::listener

#endif
