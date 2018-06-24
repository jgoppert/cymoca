#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <vector>
#include <casadi/casadi.hpp>
#include <stack>
#include <cymoca_compiler/listener/WhenExpander.h>

#include "cymoca_compiler/Compiler.h"
#include "cymoca_compiler/listener/Lisp.h"


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
    assert(iter != _expr.end());
    return *(iter->second);
  }
  ca::SX & getExpr(const ast::Node * ctx) {
    auto iter = _expr.find(ctx);
    assert(iter != _expr.end());
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
    } else if (ctx->op().compare("==") == 0) {
      e = left == right;
    } else if (ctx->op().compare("<>") == 0) {
      e = left != right;
    }
    setExpr(ctx, e);
  }
  void exit(ast::Equation *ctx) override {
    auto left = getExpr(ctx->left());
    auto right = getExpr(ctx->right());
    auto e = left - right;
    setExpr(ctx, e);
  }
  void exit(ast::WhenEquation *ctx) override {
    // TODO casadi can't handle these, preprocess first
    auto b = ctx->blocks();
    setExpr(ctx, getExpr(b[0]->condition()));
  }
  void exit(ast::IfEquation *ctx) override {
    auto blocks = ctx->blocks();
    auto e = ca::SX::if_else_zero(
        getExpr(blocks[blocks.size() - 1]->condition()),
        getExpr(blocks[blocks.size() - 1]->equations()));
    for (size_t i=blocks.size() - 1; i>0; i--) {
      e = ca::SX::if_else(
          getExpr(blocks[i]->condition()),
          getExpr(blocks[i]->equations()),
          e);
    }
    setExpr(ctx, e);
  }
  void exit(ast::EquationList *ctx) override {
    std::vector<ca::SX> eqs;
    for (auto eq: ctx->equations()) {
      assert(eq.get());
      eqs.push_back(getExpr(eq));
    }
    auto e = ca::SX::vertcat(eqs);
    setExpr(ctx, e);
  }
  void exit(ast::Class *ctx) override {
    std::vector<ca::SX> eqs;
    auto sections = ctx->equationSections();
    for (auto sec: sections) {
      eqs.push_back(getExpr(sec));
    }
    setExpr(ctx, ca::SX::vertcat(eqs));
  }
  void exit(ast::ComponentRef *ctx) override {
    setExpr(ctx, ca::SX::sym(ctx->name()));
  }
  void exit(ast::UnsignedNumber *ctx) override {
    auto e = ca::SX(ctx->val());
    setExpr(ctx, e);
  }
  void exit(ast::Derivative *ctx) override {
    auto ref = std::dynamic_pointer_cast<ast::ComponentRef>(ctx->var());
    assert(ref.get());
    setExpr(ctx, ca::SX::sym("der(" + ref->name() + ")"));
  }
  void exit(ast::Pre *ctx) override {
    auto ref = std::dynamic_pointer_cast<ast::ComponentRef>(ctx->var());
    assert(ref.get());
    setExpr(ctx, ca::SX::sym("pre(" + ref->name() + ")"));
  }
  void exit(ast::Negative *ctx) override {
    setExpr(ctx, -getExpr(ctx->expr()));
  }
};


TEST(CasadiTest, Simple) {
  {
    auto x = casadi::SX::sym("x");
    auto y = x * 2;
    auto f = casadi::Function(
        "f",
        std::vector<casadi::SX>({x}),
        std::vector<casadi::SX>({y}),
        std::vector<std::string>({"x"}),
        std::vector<std::string>({"y"})
    );
  }
  {
    path p("../../test/models/BouncingBall.mo");
    ASSERT_TRUE(exists(p));
    std::ifstream fileStream(p.string());
    cymoca::Compiler c(fileStream);
    auto tree = c.getRoot();

    CasadiListener casadiListener;
    listener::Lisp lispListener;
    cymoca::ast::Walker walker;
    walker.walk(lispListener, tree);
    std::cout << "\nlisp\n" << lispListener.get() << std::endl;
    walker.walk(casadiListener, tree);
    std::cout << "\ncasadi" << casadiListener.getExpr(tree) << std::endl;
  }
  {
    path p("../../test/models/BouncingBall.mo");
    ASSERT_TRUE(exists(p));
    std::ifstream fileStream(p.string());
    cymoca::Compiler c(fileStream);
    auto tree = c.getRoot();

    CasadiListener casadiListener;
    listener::Lisp lispListener;
    cymoca::ast::Walker walker;
    listener::WhenExpander whenExpander;
    walker.walk(whenExpander, tree);
    whenExpander.swap();
    walker.walk(lispListener, tree);
    std::cout << "\nwhen expanded\n" << lispListener.get() << std::endl;
    walker.walk(casadiListener, tree);
    std::cout << "\ncasadi" << casadiListener.getExpr(tree) << std::endl;
  }
}
