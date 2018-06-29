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
class Flattener : public SwapListener {
 public:
  void exit(const Component &ctx) override {
    ctx->t
  }
};

} // listener
} // cymoca