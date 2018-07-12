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
  void enter(condition::And &) override { m_ss << "and"; }
  void enter(condition::Boolean &ctx) override {
    m_ss << std::boolalpha << ctx.getValue();
  }
  void enter(condition::Equal &) override { m_ss << "=="; }
  void enter(condition::GreaterThan &) override { m_ss << ">"; }
  void enter(condition::GreaterThanOrEqual &) override { m_ss << ">="; }
  void enter(condition::LessThan &) override { m_ss << "<"; }
  void enter(condition::LessThanOrEqual &) override { m_ss << "<="; }
  void enter(condition::Not &) override { m_ss << "not"; }
  void enter(condition::NotEqual &) override { m_ss << "<>"; }
  void enter(condition::Or &) override { m_ss << "or"; }
  void enter(element::Component &ctx) override { m_ss << ctx.getName(); }
  void enter(equation::If &) override { m_ss << "if"; }
  void enter(equation::List &) override {}
  void enter(equation::Simple &) override { m_ss << "="; }
  void enter(equation::When &) override { m_ss << "when"; }
  void enter(expression::Add &) override { m_ss << "+"; }
  void enter(expression::Divide &) override { m_ss << "/"; }
  void enter(expression::Function &ctx) override { m_ss << "func"; }
  void enter(expression::List &) override {}
  void enter(expression::Multiply &) override { m_ss << "*"; }
  void enter(expression::Negative &) override { m_ss << "-"; }
  void enter(expression::Number &ctx) override { m_ss << ctx.getValue(); }
  void enter(expression::Reference &ctx) override { m_ss << ctx.getName(); }
  void enter(model::Class &) override { m_ss << "class"; }
  void enter(model::ElementDict &) override { m_ss << "elem dict"; }
  void enter(statement::Block &) override {}
  void enter(statement::If &) override { m_ss << "if"; }
  void enter(statement::List &) override {}
  void enter(statement::When &) override { m_ss << "when"; }
  void enterEvery(INode &) override { m_ss << "("; }
  void exitEvery(INode &) override { m_ss << ")"; }
};

}  // namespace cymoca::ast::listener

#endif
