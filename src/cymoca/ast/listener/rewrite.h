#ifndef CYMOCA_AST_LISTENER_REWRITE_H_
#define CYMOCA_AST_LISTENER_REWRITE_H_

#include "../custom.h"
#include "../node.h"
#include "../templates.h"

namespace cymoca::ast::listener {

class ApplyFunctionToReferences : public Listener {
 public:
  ApplyFunctionToReferences(expression::Reference function)
      : m_function(std::move(function)) {}
  virtual void exit(expression::Reference &ctx) {
    if (ctx.choice) {
      ctx.choice->set_option(expression::Function{m_function, {ctx}});
    }
  }

 private:
  expression::Reference m_function;
};

class ExpandWhen : public Listener {
 public:
  virtual void exit(equation::When &ctx) {
    if (ctx.choice) {
      // change condition to condition and not pre(condition)
      for (auto &b : ctx.blocks) {
        // compute pre condition
        auto pre_condition = b.condition;
        Walker().walk(pre_condition, m_apply_pre);
        b.condition =
            condition::And{b.condition, condition::Not{pre_condition}};
      }
      // replace existing choice with an if statement
      ctx.choice->set_option(equation::If{ctx.blocks});
    }
  }

 private:
  ApplyFunctionToReferences m_apply_pre{expression::Reference{"pre"}};
};

}  // namespace cymoca::ast::listener

#endif
