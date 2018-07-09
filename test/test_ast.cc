#include <gtest/gtest.h>
#include <iostream>

#include "cymoca/ast/ast.h"
#include "cymoca/ast/listener/lisp.h"
#include "test_config.h"

using namespace cymoca::ast;

// save typing
#define M(NAME) std::make_unique<NAME>
namespace elem = element;
namespace eq = equation;
namespace expr = expression;
namespace cond = condition;

struct AddOne : listener::Base {
  void enter(expr::Number &ctx) override { ctx.setValue(ctx.getValue() + 1); }
};

TEST(Ast, If) {
  auto c =
      M(ElementDict)(M(elem::Component)("x", "Real", elem::Prefix::CONSTANT),
                     M(elem::Component)("y", "Real", elem::Prefix::CONSTANT));

  auto e = M(eq::If)(
      M(eq::Block)(
          M(cond::LessThan)(M(expr::Reference)("x"), M(expr::Number)(3)),
          M(eq::Simple)(M(expr::Add)(M(expr::Number)(1), M(expr::Number)(2)),
                        M(expr::Reference)("x"))),
      M(eq::Block)(
          M(cond::And)(
              M(cond::LessThan)(M(expr::Reference)("x"), M(expr::Number)(3)),
              M(cond::Boolean)(true)),
          M(eq::Simple)(M(expr::Add)(M(expr::Number)(1), M(expr::Number)(2)),
                        M(expr::Reference)("x"))));

  auto e2 = e->cloneAs<eq::If>();

  auto e3 =
      M(ElementDict)(M(elem::Component)("z", "Real", elem::Prefix::CONSTANT));

  listener::Lisp printer;
  Walker walker;
  walker.walk(*e2, printer);

  AddOne add_one;
  walker.walk(*e2, add_one);
}
