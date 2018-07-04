//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "cymoca_compiler/ast/ast.h"
#include <sstream>

using namespace std;
using namespace cymoca::ast;

namespace cymoca::listener {

/**
 * A listener to expand when equations.
 */
class WhenExpander : public Listener {
private:
  Walker _walker{};
  class PreNamer : public Listener {
  public:
    void exit(ComponentRef &ctx) override {
      auto args = make_unique<Args>();
      args->append(ctx.cloneAs<ComponentRef>());
      auto pre = make_unique<FunctionCall>("pre", move(args));
      ctx.parent()->swapChild(ctx, move(pre));
    }
  } _preNamer{};

public:
  void exit(WhenEquation &ctx) override {
    auto ifEq = make_unique<IfEquation>();
    for (auto &block : ctx.elements()) {
      auto newEqs = make_unique<EquationList>();
      for (auto eq : block->list().elements()) {
        assert(eq->nodeType() == typeid(SimpleEquation));
        // left side is pre
        auto newEq = eq->cloneAs<SimpleEquation>();
        _walker.walk(newEq->left(), _preNamer);
        newEqs->append(move(newEq));
      }
      auto c_pre = block->condition().cloneAs<LogicExpr>();
      _walker.walk(*c_pre, _preNamer);
      // condition and not pre condition
      auto newCond = make_unique<BinaryLogicExpr>(
          block->condition().cloneAs<LogicExpr>(), BinaryLogicOp::AND,
          make_unique<UnaryLogicExpr>(UnaryLogicOp::NOT, move(c_pre)));
      ifEq->append(make_unique<EquationBlock>(move(newCond), move(newEqs)));
    }
    ctx.parent()->swapChild(ctx, move(ifEq));
  }
};

} // namespace cymoca::listener

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
