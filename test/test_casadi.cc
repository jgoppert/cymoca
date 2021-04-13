#include <gtest/gtest.h>
#include <experimental/filesystem>
#include <iostream>
#include "test_config.h"

#include <cymoca/ast/listener/lisp.h>
#include <cymoca/ast/listener/listener.h>
#include <memory>
#include "cymoca/compiler.h"

#include <casadi/casadi.hpp>

using namespace cymoca;
namespace fs = std::experimental::filesystem;
namespace ca = casadi;

namespace cymoca::ast {

class CasadiSXGen : public listener::Base {
 public:
  casadi::SX m_root{};
  std::unordered_map<INode *, casadi::SX> m_ca{};
  std::string indent(Walker *wlk) { return std::string(wlk->getDepth(), '\t'); }
  void exit(expression::Number *ctx, Walker *) override {
    m_ca[ctx] = ca::SX(ctx->getValue());
  }
  void exit(expression::Negative *ctx, Walker *) override {
    m_ca[ctx] = -m_ca[ctx->getRight()];
  }
  void exit(expression::Add *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] + m_ca[ctx->getRight()];
  }
  void exit(expression::Subtract *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] - m_ca[ctx->getRight()];
  }
  void exit(expression::Multiply *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] * m_ca[ctx->getRight()];
  }
  void exit(expression::Divide *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] / m_ca[ctx->getRight()];
  }
  void exit(condition::Boolean *ctx, Walker *) override {
    m_ca[ctx] = ca::SX(ctx->getValue());
  }
  void exit(condition::LessThan *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] < m_ca[ctx->getRight()];
  }
  void exit(condition::LessThanOrEqual *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] <= m_ca[ctx->getRight()];
  }
  void exit(condition::GreaterThan *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] > m_ca[ctx->getRight()];
  }
  void exit(condition::GreaterThanOrEqual *ctx, Walker *) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] >= m_ca[ctx->getRight()];
  }
  void exit(expression::Reference *ctx, Walker *wlk) override {
    auto name = ctx->getName();
    m_ca[ctx] = ca::SX::sym(name);
  }
  void exit(element::Component *ctx, Walker *wlk) override {
    auto name = ctx->getName();
    auto prefix = ctx->getPrefix();
    const char *type = "";
    if (prefix == element::Prefix::CONSTANT) {
      type = "constant";
    } else if (prefix == element::Prefix::DISCRETE) {
      type = "discrete";
    } else if (prefix == element::Prefix::PARAMETER) {
      type = "parameter";
    } else if (prefix == element::Prefix::VARIABLE) {
      type = "variable";
    }
    m_ca[ctx] = ca::SX::sym(name);
    std::cout << indent(wlk) << type << ": " << name << std::endl;
  }
  void exit(expression::Function *ctx, Walker *wlk) override {
    auto name = ctx->getReference().getName();
    if (name == "der") {
      auto arg0 = m_ca[(ctx->getArgs().list()[0]).get()];
      std::cout << indent(wlk) << "function: der(" << arg0 << ")" << std::endl;
      m_ca[ctx] = ca::SX::sym("der(" + arg0.get_str() + ")");
    } else {
      throw compiler_exception("not implemented");
    }
  }
  void exit(equation::Simple *ctx, Walker *wlk) override {
    m_ca[ctx] = ca::SX::eq(m_ca[ctx->getLeft()], m_ca[ctx->getRight()]);
    std::cout << indent(wlk) << "equation simple: " << m_ca[ctx] << std::endl;
  }
  void exit(equation::List *ctx, Walker *wlk) override {
    std::vector<ca::SX> eqs;
    for (auto &eq : ctx->list()) {
      eqs.push_back(m_ca[eq.get()]);
    }
    m_ca[ctx] = ca::SX::vertcat(eqs);
    std::cout << indent(wlk) << "equation list: " << m_ca[ctx] << std::endl;
  }

  void exit(model::Class *ctx, Walker *wlk) override {
    m_root = m_ca[&(ctx->getEquations())];
    std::cout << indent(wlk) << "class: " << m_root << std::endl;
  }

  void exit(equation::Block *ctx, Walker *wlk) override {
    m_ca[ctx] = m_ca[ctx->getEquations()];
    std::cout << indent(wlk) << "equation block: " << m_ca[ctx] << std::endl;
  }

  void exit(ast::condition::And *ctx, Walker *wlk) override {
    m_ca[ctx] = ca::SX::logic_and(m_ca[ctx->getLeft()], m_ca[ctx->getRight()]);
  }

  void exit(ast::condition::Equal *ctx, Walker *wlk) override {
    m_ca[ctx] = ca::SX::eq(m_ca[ctx->getLeft()], m_ca[ctx->getRight()]);
  }

  void exit(ast::condition::Not *ctx, Walker *wlk) override {
    m_ca[ctx] = ca::SX::logic_not(m_ca[ctx->getRight()]);
  }

  void exit(ast::condition::Or *ctx, Walker *wlk) override {
    m_ca[ctx] = ca::SX::logic_or(m_ca[ctx->getLeft()], m_ca[ctx->getRight()]);
  }

  void exit(equation::When *ctx, Walker *wlk) override {
    for (auto &block : ctx->list()) {
      auto cond = m_ca[block->getCondition()];
      std::vector<ca::SX> eqs;
      for (auto &eq : block->getEquations()->list()) {
        eqs.push_back(m_ca[eq.get()]);
      }
      m_ca[ctx] = ca::SX::if_else_zero(cond, ca::SX::vertcat(eqs));
    }
    std::cout << indent(wlk) << "exit equation when: " << m_ca[ctx]
              << std::endl;
  }
};

}  // namespace cymoca::ast

TEST(Casadi, Basic) {
  auto x = casadi::MX::sym("x");
  auto c = casadi::SX(3);
}

TEST(Casadi, BouncingBall) {
  std::cout << std::endl;
  fs::path p =
      fs::path(cymoca::test::path) / fs::path("models/BouncingBall.mo");
  std::cout << "path: " << p << std::endl;
  ASSERT_TRUE(exists(p));
  std::ifstream fileStream(p.string());
  Compiler c(fileStream);

  ast::listener::Lisp printer;
  ast::Walker walker(false);

  walker.walk(c.root(), printer);
  std::cout << printer.get() << std::endl;

  ast::CasadiSXGen casadiSXGen;
  walker.walk(c.root(), casadiSXGen);

  std::cout << "casadi: " << casadiSXGen.m_root << std::endl;
}
