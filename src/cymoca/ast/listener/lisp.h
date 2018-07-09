#include "listener.h"

#ifndef CYMOCA_AST_LISTENER_LISP_H_
#define CYMOCA_AST_LISTENER_LISP_H_

namespace cymoca::ast::listener {

class Lisp : public listener::Base {
 public:
  void enterEvery(INode &) override { std::cout << "("; }
  void exitEvery(INode &) override { std::cout << ")"; }
  void enter(expression::Add &) override { std::cout << "+"; }
  void enter(equation::Simple &) override { std::cout << "="; }
  void enter(equation::If &) override { std::cout << "if"; }
  void enter(condition::LessThan &) override { std::cout << "<"; }
  void enter(condition::GreaterThan &) override { std::cout << ">"; }
  void enter(condition::LessThanOrEqual &) override { std::cout << "<="; }
  void enter(condition::GreaterThanOrEqual &) override { std::cout << ">="; }
  void enter(condition::Equal &) override { std::cout << "=="; }
  void enter(expression::Number &ctx) override { std::cout << ctx.getValue(); }
  void enter(condition::Boolean &ctx) override {
    std::cout << std::boolalpha << ctx.getValue();
  }
  void enter(expression::Reference &ctx) override {
    std::cout << ctx.getName();
  }
};

}  // namespace cymoca::ast::listener

#endif
