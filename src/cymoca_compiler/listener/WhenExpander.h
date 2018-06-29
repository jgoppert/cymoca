//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include <sstream>
#include "cymoca_compiler/ast/ast.h"

using namespace std;
using namespace cymoca::ast;

namespace cymoca {
namespace listener {


/**
 * A listener to expand when equations.
 */
class WhenExpander : public SwapListener {
 private:
  Walker _walker{};
  class PreNamer : public SwapListener {
   public:
    void exit(const ComponentRef &ctx) override {
      auto args = make_unique<Args>();
      args->append(ctx.cloneAs<ComponentRef>());
      auto pre = make_unique<FunctionCall>("pre", move(args));
      setSwap(ctx, move(pre));
      swap();
    }
  } _preNamer;
 public:
  void exit(const WhenEquation &ctx) override {
    auto ifEq = make_unique<IfEquation>();
    for (auto & e: ctx.elements()) {
      auto block = e->cloneAs<EquationBlock>();
      for (auto eq: block->list().elements()) {
        auto s = static_cast<SimpleEquation *>(eq);
        _walker.walk(s->left(), _preNamer);
      }
      ifEq->append(move(block));
    }
    setSwap(ctx, move(ifEq));
    swap();
  }
};

} // listener
} // cymoca
