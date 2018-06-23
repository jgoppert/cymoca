//
// Created by jgoppert on 6/23/18.
//

#ifndef CYMOCA_LISPLISTENER_H
#define CYMOCA_LISPLISTENER_H

#include "Ast.h"

namespace cymoca {

class LispListener : public ast::Listener {
 public:
  LispListener() : _ss() {
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
  void enterEvery(ast::Node *ctx) override {
    _ss << "(";
  }

  void exitEvery(ast::Node *ctx) override {
    _ss << ")";
  }
  std::string get() {
    return _ss.str();
  }
 private:
  std::stringstream _ss;
};

}

#endif //CYMOCA_LISPLISTENER_H
