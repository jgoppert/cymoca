//
// Created by jgoppert on 6/23/18.
//

#pragma once

#include "cymoca_compiler/Ast.h"
#include "cymoca_compiler/Compiler.h"

namespace cymoca {

namespace listener {

class WhenExpander : public ast::Listener {
 private:
  std::map<ast::Node * , ast::Node::Ptr> _swap;
 public:
  WhenExpander(): _swap() {
  }
  void exit(ast::WhenEquation * ctx) override {
    // swap when(expr){ eqs }
    // with c = expr, if(c and not pre(c)) { eqs }
    auto e = std::make_shared<ast::IfEquation>();
    auto b = ctx->blocks();
    e->addBlock(b[0]);
    _swap[ctx] = e;
  }
  void swap() {
    for(auto iter=_swap.begin(); iter!=_swap.end(); iter++) {
      iter->first->parent()->swapChild(iter->first, iter->second);
    }
  }
};

}
}