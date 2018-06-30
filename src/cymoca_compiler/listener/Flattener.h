//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "cymoca_compiler/ast/ast.h"

#include <iostream>
#include <stack>


using namespace std;
using namespace cymoca::ast;

namespace cymoca {
namespace listener {

/**
 * A listener to flatten the class hierarchy.
 */
class Flattener : public Listener {
 public:
  void enter(Class &ctx) override {
    _stack.push(&ctx);
  }
  void exit(Class &ctx) override {
    _stack.pop();
  }
  void exit(Component &ctx) override {
    /*
    cout << ctx.type().name() << endl;
    if (ctx.type().name() == "Real") {
      auto c = _stack.top()->components();
      c->components().
    }
     */
    // replace component with instantiated components
    // add equations and algorithms
  }
 protected:
  stack<const Class *> _stack{};
};

} // listener
} // cymoca