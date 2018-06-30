//
// Created by jgoppert on 6/28/18.
//

#pragma once

/**
 * This class contains all of the listener hooks for nodes. It uses macros to keep the
 * code small. If you add a new node type, add it to both the Listener and ConstListener
 * class using the macros.
 */

// TODO It might be better to use a python script or similar to generate this file if the maintenance gets out of hand.

#include "declarations.h"
#include <cassert>

namespace cymoca {
namespace ast {

#define VISITOR_MACRO(NAME) \
virtual void enter(NAME & ctx) {}; \
virtual void exit(NAME & ctx) {};

class Listener {
 public:
  Listener() = default;
  virtual ~Listener() = default;
  virtual void enterEvery(Node &ctx) {};
  virtual void exitEvery(Node &ctx) {};
  VISITOR_MACRO(Args)
  VISITOR_MACRO(BinaryExpr)
  VISITOR_MACRO(BinaryLogicExpr)
  VISITOR_MACRO(Boolean)
  VISITOR_MACRO(Class)
  VISITOR_MACRO(Component)
  VISITOR_MACRO(ComponentDict)
  VISITOR_MACRO(ComponentRef)
  VISITOR_MACRO(EquationBlock)
  VISITOR_MACRO(EquationList)
  VISITOR_MACRO(FunctionCall)
  VISITOR_MACRO(IfEquation)
  VISITOR_MACRO(IfStatement)
  VISITOR_MACRO(Number)
  VISITOR_MACRO(Relation)
  VISITOR_MACRO(SimpleEquation)
  VISITOR_MACRO(SimpleStatement)
  VISITOR_MACRO(StatementBlock)
  VISITOR_MACRO(StatementList)
  VISITOR_MACRO(UnaryExpr)
  VISITOR_MACRO(UnaryLogicExpr)
  VISITOR_MACRO(WhenEquation)
  VISITOR_MACRO(WhenStatement)
};

#define VISITOR_CONST_MACRO(NAME) \
virtual void enter(const NAME & ctx) {}; \
virtual void exit(const NAME & ctx) {};

class ConstListener {
 public:
  ConstListener() = default;
  virtual ~ConstListener() = default;
  virtual void enterEvery(const Node &ctx) {};
  virtual void exitEvery(const Node &ctx) {};
  VISITOR_CONST_MACRO(Args)
  VISITOR_CONST_MACRO(BinaryExpr)
  VISITOR_CONST_MACRO(BinaryLogicExpr)
  VISITOR_CONST_MACRO(Boolean)
  VISITOR_CONST_MACRO(Class)
  VISITOR_CONST_MACRO(Component)
  VISITOR_CONST_MACRO(ComponentDict)
  VISITOR_CONST_MACRO(ComponentRef)
  VISITOR_CONST_MACRO(EquationBlock)
  VISITOR_CONST_MACRO(EquationList)
  VISITOR_CONST_MACRO(FunctionCall)
  VISITOR_CONST_MACRO(IfEquation)
  VISITOR_CONST_MACRO(IfStatement)
  VISITOR_CONST_MACRO(Number)
  VISITOR_CONST_MACRO(Relation)
  VISITOR_CONST_MACRO(SimpleEquation)
  VISITOR_CONST_MACRO(SimpleStatement)
  VISITOR_CONST_MACRO(StatementBlock)
  VISITOR_CONST_MACRO(StatementList)
  VISITOR_CONST_MACRO(UnaryExpr)
  VISITOR_CONST_MACRO(UnaryLogicExpr)
  VISITOR_CONST_MACRO(WhenEquation)
  VISITOR_CONST_MACRO(WhenStatement)
};

} // ast
} // cymoca


