//
// Created by jgoppert on 6/27/18.
//

#include <iostream>
#include <sstream>
#include <cymoca_compiler/ast/ast.h>
#include <cymoca_compiler/listener/LispPrinter.h>


using namespace std;

using namespace cymoca::ast;
using namespace cymoca::listener;

/**
 * A listener to add one to every number.
 */
class AddOne : public SwapListener {
  void exit(const Number &ctx) override {
    setSwap(ctx, make_unique<Number>(ctx.val() + 1));
    swap();
  }
};

int main() {

  auto e1 = make_unique<BinaryExpr>(
      make_unique<Number>(1),
      BinaryOp::SUB,
      make_unique<Number>(1));

  LispPrinter printer;
  Walker walker;
  walker.walk(*e1, printer);
  cout << printer.get() << endl;

  auto e1Copy = e1->cloneAs<BinaryExpr>();

  AddOne addListener;
  walker.walk(*e1, addListener);
  walker.walk(*e1, printer);
  cout << "rewrite:" << printer.get() << endl;

  walker.walk(*e1Copy, printer);
  cout << "original:" << printer.get() << endl;

  auto model = make_unique<Class>(
      make_unique<ComponentDict>(
          initializer_list<pair<string, unique_ptr<Component>>>(
              {
                  {"x", make_unique<Component>("x")},
                  {"v", make_unique<Component>("v")},
                  {"g", make_unique<Component>("g")}
              }
          )
      ),
      make_unique<List<Equation>>(
          initializer_list<unique_ptr<Equation>>(
              {
                  make_unique<SimpleEquation>(
                      make_unique<ComponentRef>("x"),
                      make_unique<ComponentRef>("v")
                  ),
                  make_unique<SimpleEquation>(
                      make_unique<ComponentRef>("v"),
                      make_unique<UnaryExpr>(
                          UnaryOp::NEG,
                          make_unique<ComponentRef>("g")
                      )
                  ),
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

  walker.walk(*model, printer);
  cout << "model:" << printer.get() << endl;
}
