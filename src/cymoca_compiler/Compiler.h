//
// Created by jgoppert on 3/17/18.
//

#pragma once

#include "ast/ast.h"
#include "util.h"
#include <memory>
#include <modelica_antlr/ModelicaBaseListener.h>
#include <modelica_antlr/ModelicaLexer.h>
#include <ostream>
#include <unordered_map>

using namespace std;
using namespace modelica_antlr;

namespace cymoca {

/**
 * This is the main compiler class.
 */
class Compiler : public ModelicaBaseListener {
public:
  Compiler(ifstream &text);
  ModelicaParser &getParser() { return *_parser; }
  antlr4::CommonTokenStream &getTokenStream() { return _tokenStream; }
  ast::Class *root() { return _root; }
  Compiler(const Compiler &) = delete;
  Compiler &operator=(const Compiler &) = delete;
  typedef unordered_map<antlr4::ParserRuleContext *, unique_ptr<ast::Node>>
      AstMap;
  const AstMap &ast() { return _ast; }

protected:
  unique_ptr<ModelicaParser> _parser;
  antlr4::ANTLRInputStream _input;
  ModelicaLexer _lexer;
  antlr4::CommonTokenStream _tokenStream;
  ast::Class *_root{nullptr};
  bool _verbose;
  AstMap _ast;

  template <typename T> unique_ptr<T> ast(antlr4::ParserRuleContext *ctx) {
    auto iter = _ast.find(ctx);
    assert(iter != _ast.end());
    unique_ptr<T> val = static_unique_ptr_cast<T>(move(iter->second));
    assert(val.get() != nullptr);
    return val;
  }

  void ast(antlr4::ParserRuleContext *ctx, unique_ptr<ast::Node> node) {
    auto iter = _ast.find(ctx);
    assert(node.get() != nullptr);
    assert(iter == _ast.end());
    _ast[ctx] = move(node);
  }

  void linkAst(antlr4::ParserRuleContext *to, antlr4::ParserRuleContext *from) {
    auto iter = _ast.find(from);
    assert(iter != _ast.end());
    ast(to, move(iter->second));
  }

  /**
   * Helper methods
   */

  string indent(int n);

  /**
   * Listener Functions
   */
public:
  void visitTerminal(antlr4::tree::TerminalNode *node) override;
  void visitErrorNode(antlr4::tree::ErrorNode *node) override;
  void enterEveryRule(antlr4::ParserRuleContext *context) override;
  void exitEveryRule(antlr4::ParserRuleContext *context) override;
  void exitComposition(ModelicaParser::CompositionContext *ctx) override;
  void
  exitExpression_simple(ModelicaParser::Expression_simpleContext *ctx) override;
  void
  exitEquation_simple(ModelicaParser::Equation_simpleContext *ctx) override;
  void
  exitArgs_expression(ModelicaParser::Args_expressionContext *ctx) override;
  void exitEquation(ModelicaParser::EquationContext *ctx) override;
  void exitWhen_equation(ModelicaParser::When_equationContext *ctx) override;
  void exitEquation_list(ModelicaParser::Equation_listContext *ctx) override;
  void exitIf_equation(ModelicaParser::If_equationContext *ctx) override;
  void exitElement_component_definition(
      ModelicaParser::Element_component_definitionContext *ctx) override;
  void exitExpr_number(ModelicaParser::Expr_numberContext *ctx) override;
  void exitExpr_unary(ModelicaParser::Expr_unaryContext *ctx) override;
  void exitExpr_binary(ModelicaParser::Expr_binaryContext *ctx) override;
  void exitExpr_ref(ModelicaParser::Expr_refContext *ctx) override;
  void exitExpr_func(ModelicaParser::Expr_funcContext *ctx) override;
  void exitExpr_output(ModelicaParser::Expr_outputContext *context) override;
};

} // namespace cymoca

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
