#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <vector>
#include <casadi/casadi.hpp>

#include "cymoca_compiler/Compiler.h"
#include "cymoca_compiler/listener/LispPrinter.h"
#include "cymoca_compiler/listener/WhenExpander.h"


using namespace boost::filesystem;
using namespace cymoca;
namespace ca = casadi;

class CasadiListener : public ast::Listener {
 private:
  size_t _depth;
  std::map<const ast::Node *, unique_ptr<ca::SX>> _expr;
  bool _verbose;
 public:
  CasadiListener(bool verbose=false) : _depth(0), _expr(), _verbose(verbose) {
  }
  ca::SX get(const ast::Node & ctx) {
    auto s = getExpr(ctx);
    _expr.clear();
    return s;
  }
  std::string indent() {
    std::stringstream ss;
    for (size_t i=0; i < _depth; i++) {
      ss << "-->";
    }
    return ss.str();
  }
  ca::SX & getExpr(const ast::Node & ctx) {
    auto iter = _expr.find(&ctx);
    assert(iter != _expr.end());
    return *(iter->second);
  }
  void setExpr(const ast::Node & ctx, const ca::SX & e) {
    auto iter = _expr.find(&ctx);
    assert(iter == _expr.end());
    _expr[&ctx] = make_unique<ca::SX>(e);
  }
  void enterEvery(const ast::Node & ctx) override {
    if (_verbose) {
      std::cout << indent() << ctx.getType().name() << " {" << std::endl;
    }
    _depth += 1;
  }
  void exitEvery(const ast::Node &ctx) override {
    if (_verbose) {
      std::cout << indent() << "} " << ctx.getType().name() << std::endl;
    }
    _depth -=1;
  }
  void exit(const ast::UnaryLogicExpr &ctx) override {
    ca::SX e;
    auto right = getExpr(ctx.right());

    if (ctx.op() == ast::UnaryLogicOp::NOT) {
      e = !right;
    } else {
      assert(false);
    }
    setExpr(ctx, e);
  }
  void exit(const ast::BinaryLogicExpr &ctx) override {
    ca::SX e;
    auto left = getExpr(ctx.left());
    auto right = getExpr(ctx.right());

    if (ctx.op() == ast::BinaryLogicOp::AND) {
      e = left && right;
    } else if (ctx.op() == BinaryLogicOp::OR) {
      e = left || right;
    }
    setExpr(ctx, e);
  }
  void exit(const ast::Relation &ctx) override {
    ca::SX e;
    auto left = getExpr(ctx.left());
    auto right = getExpr(ctx.right());

    if (ctx.op() == ast::RelationOp::LT) {
      e = left < right;
    } else if (ctx.op() == ast::RelationOp::LE) {
      e = left <= right;
    } else if (ctx.op() == ast::RelationOp::GT) {
      e = left > right;
    } else if (ctx.op() == ast::RelationOp::GE) {
      e = left >= right;
    } else if (ctx.op() == ast::RelationOp::EQ) {
      e = left == right;
    } else if (ctx.op() == ast::RelationOp::NEQ) {
      e = left != right;
    } else {
      assert(false);
    }
    setExpr(ctx, e);
  }
  void exit(const ast::SimpleEquation &ctx) override {
    auto left = getExpr(ctx.left());
    auto right = getExpr(ctx.right());
    auto e = left - right;
    setExpr(ctx, e);
  }
  void exit(const ast::WhenEquation &ctx) override {
    // TODO casadi can't handle these, preprocess first
    auto b = ctx.elements();
    setExpr(ctx, getExpr(b[0]->condition()));
  }
  void exit(const ast::IfEquation &ctx) override {
    auto blocks = ctx.elements();
    assert (blocks.size() > 0);
    auto e = ca::SX::if_else_zero(
        getExpr(blocks[blocks.size() - 1]->condition()),
        getExpr(blocks[blocks.size() - 1]->list()));
    for (size_t i=blocks.size() - 1; i>0; i--) {
      e = ca::SX::if_else(
          getExpr(blocks[i]->condition()),
          getExpr(blocks[i]->list()),
          e);
    }
    setExpr(ctx, e);
  }
  void exit(const ast::EquationList &ctx) override {
    std::vector<ca::SX> eqs;
    for (auto & eq: ctx.elements()) {
      assert(eq);
      eqs.push_back(getExpr(*eq));
    }
    auto e = ca::SX::vertcat(eqs);
    setExpr(ctx, e);
  }
  void exit(const ast::Class &ctx) override {
    std::vector<ca::SX> eqs;
    getExpr(ctx.equations());

    auto & sections = ctx.equations();
    for (auto & sec: sections.elements()) {
      eqs.push_back(getExpr(*sec));
    }
    setExpr(ctx, ca::SX::vertcat(eqs));
  }
  void exit(const ast::ComponentRef &ctx) override {
    setExpr(ctx, ca::SX::sym(ctx.name()));
  }
  void exit(const ast::Number &ctx) override {
    auto e = ca::SX(ctx.val());
    setExpr(ctx, e);
  }
  void exit(const ast::FunctionCall &ctx) override {
    auto expr = ctx.args().elements()[0];
    auto & var = static_cast<const ComponentRef &> (*expr);
    setExpr(ctx, ca::SX::sym(ctx.name() + "(" + var.name() + ")"));
  }
  void exit(const ast::UnaryExpr &ctx) override {
    setExpr(ctx, -getExpr(ctx.right()));
  }
};


TEST(CompilerTest, Casadi) {
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
    auto & tree = *c.getRoot();

    CasadiListener casadiListener;
    listener::LispPrinter lispListener;
    cymoca::ast::Walker walker;

    // original
    walker.walk(tree, lispListener);
    std::cout << "\nlisp\n" << lispListener.get() << std::endl;
    walker.walk(tree, casadiListener);
    std::cout << "\ncasadi" << casadiListener.get(tree) << std::endl;

    // apply when expander
    listener::WhenExpander whenExpander;
    walker.walk(tree, whenExpander);
    walker.walk(tree, lispListener);
    std::cout << "\nwhen expanded\n" << lispListener.get() << std::endl;
    walker.walk(tree, casadiListener);
    std::cout << "\ncasadi" << casadiListener.get(tree) << std::endl;
  }
}
