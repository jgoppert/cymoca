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
  casadi::SX m_root{ca::SX(1)};
  std::unordered_map<INode *, casadi::SX> m_ca{};
  void exit(expression::Number *ctx) override {
    m_ca[ctx] = ca::SX(ctx->getValue());
  }
  void exit(expression::Negative *ctx) override {
    m_ca[ctx] = -m_ca[ctx->getRight()];
  }
  void exit(expression::Add *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] + m_ca[ctx->getRight()];
  }
  void exit(expression::Subtract *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] - m_ca[ctx->getRight()];
  }
  void exit(expression::Multiply *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] * m_ca[ctx->getRight()];
  }
  void exit(expression::Divide *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] / m_ca[ctx->getRight()];
  }
  void exit(condition::Boolean *ctx) override {
    m_ca[ctx] = ca::SX(ctx->getValue());
  }
  void exit(condition::LessThan *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] < m_ca[ctx->getRight()];
  }
  void exit(condition::LessThanOrEqual *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] <= m_ca[ctx->getRight()];
  }
  void exit(condition::GreaterThan *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] > m_ca[ctx->getRight()];
  }
  void exit(condition::GreaterThanOrEqual *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] >= m_ca[ctx->getRight()];
  }
  void exit(expression::Reference *ctx) override {
    m_ca[ctx] = ca::SX::sym(ctx->getName());
  }
  void exit(expression::Function *ctx) override {
    if (ctx->getReference().getName() == "der") {
    }
  }
  void exit(equation::Simple *ctx) override {
    m_ca[ctx] = m_ca[ctx->getLeft()] - m_ca[ctx->getRight()];
    std::cout << "exit equation simple:" << m_ca[ctx] << std::endl;
  }
  void exit(model::Class *ctx) override {
    std::vector<ca::SX> eqs;
    for (auto &eq : ctx->getEquations().list()) {
      eqs.push_back(m_ca[eq.get()]);
    }
    m_ca[ctx] = ca::SX::vertcat(eqs);
    m_root = m_ca[ctx];
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
  ast::Walker walker;

  walker.walk(c.root(), printer);
  std::cout << printer.get() << std::endl;

  ast::CasadiSXGen casadiSXGen;
  walker.walk(c.root(), casadiSXGen);

  std::cout << "casadi: " << casadiSXGen.m_root << std::endl;
}
