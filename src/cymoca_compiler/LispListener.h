//
// Created by jgoppert on 6/23/18.
//

#ifndef CYMOCA_LISPLISTENER_H
#define CYMOCA_LISPLISTENER_H

#include "Ast.h"

namespace cymoca {

class LispListener : public ast::Listener {
 public:
  LispListener() : _ss(), _depth(0) {
  }
  std::string indent() {
    std::stringstream ss;
    for (size_t i=0; i < _depth; i++) {
      ss << " ";
    }
    return ss.str();
  }
  void enterEvery(ast::Node *ctx) override {
    _ss << "(";
    _depth +=1;
  }
  void exitEvery(ast::Node *ctx) override {
    _depth -=1;
    _ss << ")";
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
    auto a = std::dynamic_pointer_cast<ast::Equation>(ctx->equations()[0]->equations()[0]);
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
 private:
  std::stringstream _ss;
  size_t _depth;
};

}

#endif //CYMOCA_LISPLISTENER_H
