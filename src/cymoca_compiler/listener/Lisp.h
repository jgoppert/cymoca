//
// Created by jgoppert on 6/23/18.
//

#pragma once

#include "cymoca_compiler/Ast.h"

namespace cymoca {

namespace listener {

class Lisp : public ast::Listener {
 private:
  std::stringstream _ss;
  size_t _depth;
  bool _pretty;
 public:
  Lisp() : _ss(), _depth(0), _pretty(false) {
  }
  std::string indent() {
    std::stringstream ss;
    if (_pretty) {
      ss << "\n";
      for (size_t i=0; i < _depth; i++) {
        ss << "  ";
      }
    }
    return ss.str();
  }
  void enterEvery(ast::Node *ctx) override {
    _ss << indent() << "(";
    _depth += 1;
  }
  void exitEvery(ast::Node *ctx) override {
    _depth -=1;
    _ss << indent() << ")";
  }
  void enter(ast::EquationList *ctx) override {
    _ss << "eq_list";
  }
  void enter(ast::EquationBlock *ctx) override {
    _ss << "eq_block";
  }
  void enter(ast::UnsignedNumber *ctx) override {
    _ss << ctx->val();
  }
  void enter(ast::Negative *ctx) override {
    _ss << "-";
  }
  void enter(ast::ComponentRef *ctx) override {
    _ss << ctx->name();
  }
  void enter(ast::Equation *ctx) override {
    _ss << "=";
  }
  void enter(ast::Derivative *ctx) override {
    _ss << "der";
  }
  void enter(ast::Relation *ctx) override {
    _ss << ctx->op();
  }
  void enter(ast::Component *ctx) override {
    _ss << "component";
  }
  void enter(ast::WhenEquation *ctx) override {
    _ss << "when";
  }
  void enter(ast::IfEquation *ctx) override {
    _ss << "if";
  }
  void enter(ast::Class *ctx) override {
    _ss << "class";
  }
  std::string get() {
    std::string s = _ss.str();
    _ss.str("");
    return s;
  }
};

}
}
