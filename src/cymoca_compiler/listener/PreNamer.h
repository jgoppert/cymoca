//
// Created by jgoppert on 6/24/18.
//

#pragma once

#include "cymoca_compiler/Ast.h"

namespace cymoca {

namespace listener {

class PreNamer : public ast::Listener {
 public:
  void enter(ast::ComponentRef *ctx) override {
    if (ctx->parent()->getType() != typeid(ast::Pre)) {
      auto e = std::make_shared<ast::Pre>(
          std::make_shared<ast::ComponentRef>(ctx->name())
      );
      ctx->parent()->swapChild(ctx, e);
    }
  }
};

}

}