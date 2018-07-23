#ifndef CYMOCA_AST_LISTENER_PRINT_H_
#define CYMOCA_AST_LISTENER_PRINT_H_

#include "../custom.h"
#include "../node.h"
#include "../templates.h"

#include <sstream>

namespace cymoca::ast::listener {

class PrintLisp : public Listener {
 public:
  std::string get() {
    auto s = m_ss.str();
    m_ss.str("");
    return s;
  }
  virtual void enter_every(Node &) { m_ss << "("; }
  virtual void exit_every(Node &) { m_ss << ")"; }
  virtual void enter(model::Class &) { m_ss << "class"; }
  virtual void enter(expression::Number &ctx) { m_ss << ctx.value; }
  virtual void enter(expression::Add &) { m_ss << "+"; }
  virtual void enter(expression::Subtract &) { m_ss << "-"; }
  virtual void enter(expression::Multiply &) { m_ss << "*"; }
  virtual void enter(expression::Divide &) { m_ss << "/"; }
  virtual void enter(expression::Function &) { m_ss << ""; }
  virtual void enter(equation::If &) { m_ss << "if"; }
  virtual void enter(equation::When &) { m_ss << "when"; }
  virtual void enter(equation::Simple &) { m_ss << "="; }
  virtual void enter(condition::And &) { m_ss << "and"; }
  virtual void enter(condition::Or &) { m_ss << "or"; }
  virtual void enter(condition::Not &) { m_ss << "not"; }
  virtual void enter(condition::Less &) { m_ss << "<"; }
  virtual void enter(condition::LessOrEqual &) { m_ss << "<="; }
  virtual void enter(condition::Greater &) { m_ss << ">"; }
  virtual void enter(condition::GreaterOrEqual &) { m_ss << ">="; }
  virtual void enter(condition::Equal &) { m_ss << "=="; }
  virtual void enter(expression::Reference &ctx) {
    for (size_t i = 0; i < ctx.value.size(); i++) {
      m_ss << ctx.value[i];
      if (i < ctx.value.size() - 1) {
        m_ss << ".";
      }
    }
  }
  virtual void enter(condition::Bool &ctx) {
    m_ss << std::boolalpha << ctx.value;
  }

 private:
  std::stringstream m_ss{};
};

}  // namespace cymoca::ast::listener

#endif
