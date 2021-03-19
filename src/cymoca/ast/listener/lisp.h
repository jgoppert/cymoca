#ifndef CYMOCA_AST_LISTENER_LISP_H_
#define CYMOCA_AST_LISTENER_LISP_H_

#include "../ast.h"
#include "listener.h"

namespace cymoca::ast::listener {

class Lisp : public listener::Base {
 protected:
  std::stringstream m_ss{};

 public:
  std::string get() {
    std::string s = m_ss.str();
    m_ss.str("");
    return s;
  }
  void enter(condition::And *, Walker *) override { m_ss << "and"; }
  void enter(condition::Boolean *ctx, Walker *) override {
    m_ss << std::boolalpha << ctx->getValue();
  }
  void enter(condition::Equal *, Walker *) override { m_ss << "=="; }
  void enter(condition::GreaterThan *, Walker *) override { m_ss << ">"; }
  void enter(condition::GreaterThanOrEqual *, Walker *) override {
    m_ss << ">=";
  }
  void enter(condition::LessThan *, Walker *) override { m_ss << "<"; }
  void enter(condition::LessThanOrEqual *, Walker *) override { m_ss << "<="; }
  void enter(condition::Not *, Walker *) override { m_ss << "not"; }
  void enter(condition::NotEqual *, Walker *) override { m_ss << "<>"; }
  void enter(condition::Or *, Walker *) override { m_ss << "or"; }
  void enter(element::Component *ctx, Walker *) override {
    m_ss << ctx->getName();
  }
  void enter(equation::If *, Walker *) override { m_ss << "if"; }
  void enter(equation::List *, Walker *) override {}
  void enter(equation::Simple *, Walker *) override { m_ss << "="; }
  void enter(equation::When *, Walker *) override { m_ss << "when"; }
  void enter(expression::Add *, Walker *) override { m_ss << "+"; }
  void enter(expression::Divide *, Walker *) override { m_ss << "/"; }
  void enter(expression::Function *ctx, Walker *) override { m_ss << "func"; }
  void enter(expression::List *, Walker *) override {}
  void enter(expression::Multiply *, Walker *) override { m_ss << "*"; }
  void enter(expression::Negative *, Walker *) override { m_ss << "-"; }
  void enter(expression::Number *ctx, Walker *) override {
    m_ss << ctx->getValue();
  }
  void enter(expression::Reference *ctx, Walker *) override {
    m_ss << ctx->getName();
  }
  void enter(model::Class *, Walker *) override { m_ss << "class"; }
  void enter(model::ElementDict *, Walker *) override { m_ss << "elem dict"; }
  void enter(statement::Block *, Walker *) override {}
  void enter(statement::If *, Walker *) override { m_ss << "if"; }
  void enter(statement::List *, Walker *) override {}
  void enter(statement::When *, Walker *) override { m_ss << "when"; }
  void enterEvery(INode *, Walker *) override { m_ss << "("; }
  void exitEvery(INode *, Walker *) override { m_ss << ")"; }
};

}  // namespace cymoca::ast::listener

#endif
