//
// Created by jgoppert on 3/17/18.
//

#pragma once

#include <modelica_antlr/ModelicaBaseListener.h>
#include <modelica_antlr/ModelicaLexer.h>
#include <typeindex>
#include <memory>
#include <stack>
#include <ostream>
#include <map>
#include <list>
#include "Ast.h"

using namespace modelica_antlr;

namespace cymoca {

/**
 * This is the main compiler class.
 */
class Compiler : public ModelicaBaseListener {
 public:
  Compiler(std::ifstream &text);
  virtual ~Compiler() {};
  ModelicaParser &getParser() { return *_parser; }
  antlr4::CommonTokenStream &getTokenStream() { return _tokenStream; }
  ast::Class::Ptr getRoot() { return _root; }

  typedef std::unordered_map<antlr4::ParserRuleContext *, ast::Node::Ptr> AstMap;
  const AstMap & getAst() { return _ast; }

 protected:
  std::shared_ptr<ModelicaParser> _parser;
  antlr4::ANTLRInputStream _input;
  ModelicaLexer _lexer;
  antlr4::CommonTokenStream _tokenStream;
  ast::Class::Ptr _root;
  bool _verbose;
  AstMap _ast;

  template<typename T>
  std::shared_ptr<T> getAst(antlr4::ParserRuleContext *ctx) {
    auto iter = _ast.find(ctx);
    assert(iter != _ast.end());
    auto res = iter->second;
    std::shared_ptr<T> val = std::dynamic_pointer_cast<T>(iter->second);
    assert(val.get() != nullptr);
    return val;
  }

  void setAst(antlr4::ParserRuleContext *ctx, std::shared_ptr<ast::Node> node) {
    auto iter = _ast.find(ctx);
    assert(node.get() != nullptr);
    assert(iter == _ast.end());
    _ast[ctx] = node;
  }

  void linkAst(antlr4::ParserRuleContext *to, antlr4::ParserRuleContext *from) {
    auto iter = _ast.find(from);
    assert(iter != _ast.end());
    setAst(to, iter->second);
  }

  /**
   * Helper methods
   */

  std::string indent(int n);

  /**
   * Listener Functions
   */
 public:
  void visitTerminal(antlr4::tree::TerminalNode *node) override;
  void visitErrorNode(antlr4::tree::ErrorNode *node) override;
  void enterEveryRule(antlr4::ParserRuleContext *ctx) override;
  void exitEveryRule(antlr4::ParserRuleContext *ctx) override;
  void exitPrimary_unsigned_number(ModelicaParser::Primary_unsigned_numberContext *context) override;
  void exitExpr_negative(ModelicaParser::Expr_negativeContext *context) override;
  void exitPrimary_component_reference(ModelicaParser::Primary_component_referenceContext *context) override;
  void exitComponent_declaration(ModelicaParser::Component_declarationContext *context) override;
  void exitComposition(ModelicaParser::CompositionContext *context) override;
  void exitExpression_simple(ModelicaParser::Expression_simpleContext *context) override;
  void exitSimple_expression(ModelicaParser::Simple_expressionContext *context) override;
  void exitEquation_simple(ModelicaParser::Equation_simpleContext *context) override;
  void exitPrimary_der(ModelicaParser::Primary_derContext *context) override;
  void exitArgs_expression(ModelicaParser::Args_expressionContext *context) override;
  void exitExpr_relation(ModelicaParser::Expr_relationContext *context) override;
  void exitPrimary_output_expression_list(ModelicaParser::Primary_output_expression_listContext *context) override;
  void exitOutput_expression_list(ModelicaParser::Output_expression_listContext *context) override;
  void exitEquation(ModelicaParser::EquationContext *context) override;
  void exitWhen_equation(ModelicaParser::When_equationContext *context) override;
  void exitEquation_options(ModelicaParser::Equation_optionsContext *context) override;
  void exitStatement_options(ModelicaParser::Statement_optionsContext *context) override;
  void exitEquation_list(ModelicaParser::Equation_listContext *context) override;
  void exitIf_equation(ModelicaParser::If_equationContext *context) override;
};

} // cymoca