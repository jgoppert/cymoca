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
 public:
  void exit(const WhenEquation &ctx) override {
    auto ifEq = make_unique<IfEquation>();
    for (auto & e: ctx.elements()) {
      auto block = static_unique_ptr_cast<EquationBlock >(e->clone());
      ifEq->append(move(block));
    }
    setSwap(ctx, move(ifEq));
  }
};

} // listener
} // cymoca
