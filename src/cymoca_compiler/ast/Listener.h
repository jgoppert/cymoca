//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "declarations.h"

namespace cymoca {
namespace ast {

#define VISITOR_MACRO(NAME) \
virtual void enter(const NAME & ctx) {}; \
virtual void exit(const NAME & ctx) {};

class Listener {
 public:
  Listener() = default;
  virtual ~Listener() = default;
  virtual void enterEvery(const Node &ctx) {};
  virtual void exitEvery(const Node &ctx) {};
  VISITOR_MACRO(Number)
  VISITOR_MACRO(Boolean)
  VISITOR_MACRO(BinaryExpr)
  VISITOR_MACRO(BinaryLogicExpr)
  VISITOR_MACRO(Class)
  VISITOR_MACRO(Component)
  VISITOR_MACRO(ComponentRef)
  VISITOR_MACRO(SimpleEquation)
  VISITOR_MACRO(UnaryExpr)
  VISITOR_MACRO(UnaryLogicExpr)
  VISITOR_MACRO(ComponentDict)
  VISITOR_MACRO(EquationList)
  VISITOR_MACRO(EquationBlock)
  VISITOR_MACRO(StatementList)
  VISITOR_MACRO(StatementBlock)
  VISITOR_MACRO(Relation)
  VISITOR_MACRO(WhenEquation)
  VISITOR_MACRO(WhenStatement)
  VISITOR_MACRO(IfEquation)
  VISITOR_MACRO(IfStatement)
  VISITOR_MACRO(SimpleStatement)
  VISITOR_MACRO(FunctionCall)
  VISITOR_MACRO(Args)
};

} // ast
} // cymoca


