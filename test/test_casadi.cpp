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
 private:
  typedef std::shared_ptr<ca::SX> SXPtr;
  size_t _depth;
  std::map<const ast::Node *, SXPtr> _expr;
  bool _verbose;
 public:
  CasadiListener(bool verbose=false) : _depth(0), _expr(), _verbose(verbose) {
  }
  std::string indent() {
    std::stringstream ss;
    for (size_t i=0; i < _depth; i++) {
      ss << "-->";
    }
    return ss.str();
  }
  ca::SX & getExpr(const ast::Node::Ptr & ctx) {
    auto iter = _expr.find(ctx.get());
    assert(!(iter == _expr.end()));
    return *(iter->second);
  }
  ca::SX & getExpr(const ast::Node * ctx) {
    auto iter = _expr.find(ctx);
    assert(!(iter == _expr.end()));
    return *(iter->second);
  }
  void setExpr(ast::Node * ctx, const ca::SX & e) {
    auto iter = _expr.find(ctx);
    assert(iter == _expr.end());
    _expr[ctx] = std::make_shared<ca::SX>(e);
  }
  void enterEvery(ast::Node *ctx) override {
    if (_verbose) {
      std::cout << indent() << ctx->getType().name() << " {" << std::endl;
    }
    _depth += 1;
  }
  void exitEvery(ast::Node *ctx) override {
    if (_verbose) {
      std::cout << indent() << "} " << ctx->getType().name() << std::endl;
    }
    _depth -=1;
  }
  void exit(ast::Relation *ctx) override {
    ca::SX e;
    auto left = getExpr(ctx->left());
    auto right = getExpr(ctx->right());
    if (ctx->op().compare("<") == 0) {
      e = left < right;
    } else if (ctx->op().compare("<=") == 0) {
      e = left <= right;
    } else if (ctx->op().compare(">") == 0) {
      e = left > right;
    } else if (ctx->op().compare(">=") == 0) {
      e = left >= right;
    }
    setExpr(ctx, e);
  }
  void exit(ast::Equation *ctx) override {
    setExpr(ctx, getExpr(ctx->left()) - getExpr(ctx->right()));
  }
  void exit(ast::WhenEquation *ctx) override {
    // TODO casadi can't handle these, preprocess first
    setExpr(ctx, getExpr(ctx->condition()[0]));
  }
  void exit(ast::IfEquation *ctx) override {
    // TODO casadi can't handle these, preprocess first
    setExpr(ctx, getExpr(ctx->condition()[0]));
  }
  void exit(ast::EquationList *ctx) override {
    std::vector<ca::SX> eqs;
    for (auto eq: ctx->equations()) {
      eqs.push_back(getExpr(eq));
    }
    setExpr(ctx, ca::SX::vertcat(eqs));
  }
  void exit(ast::Class *ctx) override {
    std::vector<ca::SX> eqs;
    for (auto sec: ctx->equationSection()) {
      eqs.push_back(getExpr(sec));
    }
    setExpr(ctx, ca::SX::vertcat(eqs));
  }
  void exit(ast::ComponentRef *ctx) override {
    setExpr(ctx, ca::SX::sym(ctx->name()));
  }
  void exit(ast::UnsignedNumber *ctx) override {
    setExpr(ctx, ca::SX(ctx->val()));
  }
  void exit(ast::Derivative *ctx) override {
    auto ref = std::dynamic_pointer_cast<ast::ComponentRef>(ctx->var());
    assert(ref.get() != nullptr);
    setExpr(ctx, ca::SX::sym("der(" + ref->name() + ")"));
  }
  void exit(ast::Negative *ctx) override {
    setExpr(ctx, -getExpr(ctx->expr()));
  }
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
  std::cout << "\ncasadi" << casadiListener.getExpr(c.getRoot()) << std::endl;
}
