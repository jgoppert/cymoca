#ifndef CYMOCA_AST_LISTENER_MISC_H_
#define CYMOCA_AST_LISTENER_MISC_H_

#include "../custom.h"
#include "../node.h"
#include "../templates.h"

namespace cymoca::ast::listener {

class AddOne : public Listener {
 public:
  virtual void enter(expression::Number &ctx) { ctx.value += 1; }
};

}  // namespace cymoca::ast::listener

#endif
