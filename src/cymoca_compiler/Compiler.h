//
// Created by jgoppert on 3/17/18.
//

#ifndef CYMOCA_COMPILER_H
#define CYMOCA_COMPILER_H

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
  explicit Compiler(std::ifstream &text);
  ModelicaParser &getParser() { return *_parser; }
  antlr4::CommonTokenStream &getTokenStream() { return _tokenStream; }
  ModelicaParser::Stored_definitionContext *getRoot() { return _root; }

 protected:
  antlr4::ANTLRInputStream _input;
  std::shared_ptr<ModelicaParser> _parser;
  ModelicaLexer _lexer;
  antlr4::CommonTokenStream _tokenStream;
  ModelicaParser::Stored_definitionContext *_root;

  std::unordered_map<antlr4::ParserRuleContext *, void *> _mem;
  std::unordered_map<antlr4::ParserRuleContext *, std::shared_ptr<ast::Node>> _ast;

  std::stack<ast::Class> _classStack;

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
  void exitPrimary_compoment_reference(ModelicaParser::Primary_compoment_referenceContext *context) override;
  void exitComponent_declaration(ModelicaParser::Component_declarationContext *context) override;
  void exitComposition(ModelicaParser::CompositionContext *context) override;
  void exitExpression_simple(ModelicaParser::Expression_simpleContext *context) override;
  void exitSimple_expression(ModelicaParser::Simple_expressionContext *context) override;
  void exitEquation_simple(ModelicaParser::Equation_simpleContext *context) override;
};

} // cymoca

#endif //CYMOCA_COMPILER_H
