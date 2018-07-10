#include "listener.h"

#ifndef CYMOCA_AST_LISTENER_LISP_H_
#define CYMOCA_AST_LISTENER_LISP_H_

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
  void enter(condition::Boolean &ctx) override {
    m_ss << std::boolalpha << ctx.getValue();
  }
  void enter(condition::Equal &) override { m_ss << "=="; }
  void enter(condition::GreaterThan &) override { m_ss << ">"; }
  void enter(condition::GreaterThanOrEqual &) override { m_ss << ">="; }
  void enter(condition::LessThan &) override { m_ss << "<"; }
  void enter(condition::LessThanOrEqual &) override { m_ss << "<="; }
  void enter(model::Class &) override { m_ss << "class"; }
  void enter(model::ElementDict &) override { m_ss << "elem dict"; }
  void enter(equation::List &) override { m_ss << "eq list"; }
  void enter(equation::If &) override { m_ss << "if"; }
  void enter(equation::Simple &) override { m_ss << "="; }
  void enter(expression::Add &) override { m_ss << "+"; }
  void enter(expression::Number &ctx) override { m_ss << ctx.getValue(); }
  void enter(expression::Reference &ctx) override { m_ss << ctx.getName(); }
  void enterEvery(INode &) override { m_ss << "("; }
  void exitEvery(INode &) override { m_ss << ")"; }
};

}  // namespace cymoca::ast::listener

#endif
