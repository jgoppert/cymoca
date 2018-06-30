//
// Created by jgoppert on 6/27/18.
//

#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

#include <cymoca_compiler/ast/ast.h>
#include <cymoca_compiler/listener/LispPrinter.h>
#include "cymoca_compiler/listener/WhenExpander.h"
#include "cymoca_compiler/listener/Flattener.h"


using namespace std;

using namespace cymoca::ast;
using namespace cymoca::listener;

/**
 * A listener to add one to every number.
 */
class AddOne : public Listener {
  void exit(Number &ctx) override {
    ctx.val() += 1;
  }
};

TEST(Ast, Basic) {

  auto e1 = make_unique<BinaryExpr>(
      make_unique<Number>(1),
      BinaryOp::SUB,
      make_unique<Number>(1));

  LispPrinter lispListener;
  Walker walker;
  walker.walk(*e1, lispListener);
  cout << lispListener.get() << endl;

  auto e1Copy = e1->cloneAs<BinaryExpr>();

  AddOne addListener;
  walker.walk(*e1, addListener);
  walker.walk(*e1, lispListener);
  cout << "rewrite:" << lispListener.get() << endl;

  walker.walk(*e1Copy, lispListener);
  cout << "original:" << lispListener.get() << endl;

  auto model = make_unique<Class>(
      make_unique<ComponentDict>(
          initializer_list<pair<string, unique_ptr<Component>>>(
              {
                  {"x", make_unique<Component>("x", "Real", Prefix::VARIABLE)},
                  {"v", make_unique<Component>("v", "Real", Prefix::VARIABLE)},
                  {"g", make_unique<Component>("g", "Real", Prefix::PARAMETER)}
              }
          )
      ),
      make_unique<List<Equation>>(
          initializer_list<unique_ptr<Equation>>(
              {
                  // der(x) = v
                  make_unique<SimpleEquation>(
                      make_unique<FunctionCall>("der",make_unique<ComponentRef>("x")),
                      make_unique<ComponentRef>("v")
                  ),
                  // der(v) = -g
                  make_unique<SimpleEquation>(
                      make_unique<FunctionCall>("der", make_unique<ComponentRef>("v")),
                      make_unique<UnaryExpr>(
                          UnaryOp::NEG,
                          make_unique<ComponentRef>("g")
                      )
                  ),
                  // when (x<0) v = -v
                  make_unique<WhenEquation>(
                      initializer_list<unique_ptr<EquationBlock>>(
                          {
                              make_unique<EquationBlock>(
                                  make_unique<Relation>(
                                      make_unique<ComponentRef>("x"),
                                      RelationOp::LT,
                                      make_unique<Number>(0)
                                  ),
                                  make_unique<EquationList>(
                                      initializer_list<unique_ptr<Equation>>(
                                          {
                                              make_unique<SimpleEquation>(
                                                  make_unique<ComponentRef>("v"),
                                                  make_unique<UnaryExpr>(
                                                      UnaryOp::NEG,
                                                      make_unique<ComponentRef>("v")
                                                  )
                                              )
                                          }
                                      )
                                  )
                              )
                          }
                      )
                  )
              }
          )
      )
  );

  walker.walk(*model, lispListener);
  cout << "model:" << lispListener.get() << endl;

  // apply when expander
  WhenExpander whenExpander;
  walker.walk(*model, whenExpander);
  walker.walk(*model, lispListener);
  cout << "\nwhen expanded\n" << lispListener.get() << endl;

  // apply flattener
  Flattener flattener;
  walker.walk(*model, flattener);
  walker.walk(*model, lispListener);
  cout << "\nflattened\n" << lispListener.get() << endl;


}
