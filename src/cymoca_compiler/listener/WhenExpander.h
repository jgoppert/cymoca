//
// Created by jgoppert on 6/23/18.
//

#pragma once

#include "cymoca_compiler/Ast.h"

namespace cymoca {

namespace listener {

class WhenExpander : public ast::Listener {
 public:
  WhenExpander() {
  }
  void exit(ast::WhenEquation * ctx) override {
    // replace when(expr){ eqs }
    // with c = expr, if(c and not pre(c)) { eqs }
    auto e = ast::Pre(ctx->condition()[0]);
    ctx->parent();
  }
};

}
}