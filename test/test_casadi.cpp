#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <vector>
#include <casadi/casadi.hpp>
#include <stack>

#include "cymoca_compiler/Compiler.h"
#include "cymoca_compiler/LispListener.h"


using namespace boost::filesystem;
using namespace cymoca;
namespace ca = casadi;

class CasadiListener : public ast::Listener {
 public:
  CasadiListener() : _stack() {
  }
  void enterEvery(ast::Node *ctx) override {
  }
  void exitEvery(ast::Node *ctx) override {
  }
  void exit(ast::UnsignedNumber *ctx) override {
    ca::SX val(ctx->val());
    _stack.push(val);
    std::cout << "val" << val;
  }
  void exit(ast::Negative *ctx) override {
    _stack.top() *= -1;
    std::cout << "val" << _stack.top();
  }
  ca::SX get() {
    return _stack.top();
  }
 private:
  std::stack<ca::SX> _stack;
};


TEST(CasadiTest, Simple) {
  std::cout << std::endl;
  auto x = casadi::SX::sym("x");
  auto y = x*2;
  auto f = casadi::Function(
      "f",
      std::vector<casadi::SX>({x}),
      std::vector<casadi::SX>({y}),
      std::vector<std::string>({"x"}),
      std::vector<std::string>({"y"})
  );

  std::cout << std::endl;
  path p("../../test/models/BouncingBall.mo");
  ASSERT_TRUE(exists(p));
  std::ifstream fileStream(p.string());
  cymoca::Compiler c(fileStream);
  //std::cout << s<< std::endl;

  CasadiListener casadiListener;
  LispListener lispListener;

  cymoca::ast::Walker walker;

  walker.walk(&lispListener, c.getRoot().get());
  std::cout << "\nlisp" << lispListener.get() << std::endl;
  walker.walk(&casadiListener, c.getRoot().get());
  std::cout << "\ncasadi" << casadiListener.get() << std::endl;
}
