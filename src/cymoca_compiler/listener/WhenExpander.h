//
// Created by jgoppert on 6/23/18.
//

#pragma once

#include "cymoca_compiler/Ast.h"
#include "cymoca_compiler/Compiler.h"

namespace cymoca {

namespace listener {

class PreNamer : public ast::Listener {
 public:
  void enter(ast::ComponentRef * ctx) override {
    //TODO should limit to discrete variables?
    if (ctx->parent()->getType() != typeid(ast::Pre)) {
      auto e = std::make_shared<ast::Pre>(
          std::make_shared<ast::ComponentRef>(ctx->name())
          );
      ctx->parent()->swapChild(ctx, e);
    }
  }
};

class WhenExpander : public ast::Listener {
 private:
  ast::Walker _walker;
  PreNamer _preNamer;
 public:
  void exit(ast::WhenEquation * ctx) override {
    // swap when(expr){ eqs }
    // with c = expr, if(c and not pre(c)) { eqs }
    auto e = std::make_shared<ast::IfEquation>();
    auto blocks = ctx->blocks();

    for (auto &block: blocks) {

      // set cond to expr and not pre(expr)
      //auto cond = block->condition();

      //auto newBlock = std::make_shared<ast::EquationBlock>(block->condition(), block->equations());
      //auto c = cond.copy();

      //_walker.walk(_preNamer, eq->right());

      // rename rhs of when equations to pre
      for (auto &node: block->equations()->equations()) {
        auto eq = std::dynamic_pointer_cast<ast::Equation>(node);
        _walker.walk(_preNamer, eq->right());
      }
      e->addBlock(block);
    }
    ctx->parent()->swapChild(ctx, e);
  }
};

}
}