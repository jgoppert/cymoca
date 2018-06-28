//
// Created by jgoppert on 6/28/18.
//

#pragma once

namespace cymoca {
namespace ast {

// forward declarations
class Class;
class Component;
class ComponentRef;
class Node;
class Expr;
class LogicExpr;
class Equation;
class FunctionCall;
class Statement;
class SimpleEquation;
class SimpleStatement;
class Listener;

// used to differentiate the templates
class LabelIf;
class LabelWhen;

template<class T, class Base=Expr>
class Value;

template<class Base, class Term, class Op>
class Binary;

template<class Base, class Term, class Op>
class Unary;

template<class Term>
class Dict;

template<class Term, class Base=Node, class Label=Node>
class List;

template<class Term>
class ConditionBlock;

enum class BinaryOp { ADD, SUB, MUL, DIV, POW, EMUL, EDIV, EPOW };
enum class UnaryOp { NEG };
enum class RelationOp { LT, LE, GT, GE, EQ, NEQ };
enum class BinaryLogicOp { AND, OR };
enum class UnaryLogicOp { NOT };

using ComponentDict = Dict<Component>;
using EquationList = List<Equation>;
using EquationBlock = ConditionBlock<Equation>;
using StatementList = List<Statement>;
using StatementBlock = ConditionBlock<Statement>;
using IfEquation = List<ConditionBlock<Equation>, Equation, LabelIf>;
using WhenEquation = List<ConditionBlock<Equation>, Equation, LabelWhen>;
using IfStatement = List<ConditionBlock<Statement>, Statement, LabelIf>;
using WhenStatement = List<ConditionBlock<Statement>, Statement, LabelWhen>;
using BinaryExpr = Binary<Expr, Expr, BinaryOp>;
using UnaryExpr = Unary<Expr, Expr, UnaryOp>;
using Relation = Binary<LogicExpr, Expr, RelationOp>;
using BinaryLogicExpr = Binary<LogicExpr, LogicExpr, BinaryLogicOp>;
using UnaryLogicExpr = Unary<LogicExpr, LogicExpr, UnaryLogicOp>;
using Number = Value<double>;
using Boolean = Value<bool, LogicExpr>;
using Args = List<Expr>;

} // ast
} // cymoca