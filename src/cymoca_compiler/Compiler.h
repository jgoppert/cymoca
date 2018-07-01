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
  explicit Compiler(ifstream &text);
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
    assert(val != nullptr);
    return val;
  }

  void ast(antlr4::ParserRuleContext *ctx, unique_ptr<ast::Node> node) {
    auto iter = _ast.find(ctx);
    assert(node != nullptr);
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
  void exitElement_component_definition(
      ModelicaParser::Element_component_definitionContext *ctx) override;
  // expressions
  void exitExpr_number(ModelicaParser::Expr_numberContext *ctx) override;
  void exitExpr_unary(ModelicaParser::Expr_unaryContext *ctx) override;
  void exitExpr_binary(ModelicaParser::Expr_binaryContext *ctx) override;
  void exitExpr_ref(ModelicaParser::Expr_refContext *ctx) override;
  void exitExpr_func(ModelicaParser::Expr_funcContext *ctx) override;
  void exitExpr_output(ModelicaParser::Expr_outputContext *context) override;
  // equations
  void exitEq_simple(ModelicaParser::Eq_simpleContext *context) override;
  void exitEq_if(ModelicaParser::Eq_ifContext *context) override;
  void exitEq_for(ModelicaParser::Eq_forContext *context) override;
  void exitEq_connect(ModelicaParser::Eq_connectContext *context) override;
  void exitEq_when(ModelicaParser::Eq_whenContext *context) override;
  void exitEq_func(ModelicaParser::Eq_funcContext *context) override;
  // statements
  void exitStmt_ref(ModelicaParser::Stmt_refContext *ctx) override;
  void exitStmt_func(ModelicaParser::Stmt_funcContext *ctx) override;
  void exitStmt_keyword(ModelicaParser::Stmt_keywordContext *ctx) override;
  void exitStmt_if(ModelicaParser::Stmt_ifContext *ctx) override;
  void exitStmt_for(ModelicaParser::Stmt_forContext *ctx) override;
  void exitStmt_while(ModelicaParser::Stmt_whileContext *ctx) override;
  void exitStmt_when(ModelicaParser::Stmt_whenContext *ctx) override;
  // misc
  void exitEq_block(ModelicaParser::Eq_blockContext *ctx) override;
  void exitStmt_block(ModelicaParser::Stmt_blockContext *context) override;
  void exitArgs_expr(ModelicaParser::Args_exprContext *context) override;
};

} // namespace cymoca

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
