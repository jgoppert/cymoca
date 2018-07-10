//
// Created by jgoppert on 3/17/18.
//

#ifndef CYMOCA_COMPILER_H_
#define CYMOCA_COMPILER_H_

#include <memory>
#include <ostream>
#include <unordered_map>
#include "ast/ast.h"
#include "util.h"

#include <modelica_antlr/ModelicaBaseListener.h>
#include <modelica_antlr/ModelicaLexer.h>

using namespace modelica_antlr;

namespace cymoca {

/**
 * This is the main compiler class.
 */
class Compiler : public ModelicaBaseListener {
 public:
  explicit Compiler(std::ifstream &text);
  ModelicaParser &getParser() { return *m_parser; }
  antlr4::CommonTokenStream &getTokenStream() { return m_token_stream; }
  ast::model::Class *root() { return m_root; }
  Compiler(const Compiler &) = delete;
  Compiler &operator=(const Compiler &) = delete;
  using AstMap = std::unordered_map<antlr4::ParserRuleContext *,
                             std::unique_ptr<ast::INode>>;

 protected:
  std::unique_ptr<ModelicaParser> m_parser;
  antlr4::ANTLRInputStream m_input;
  ModelicaLexer m_lexer;
  antlr4::CommonTokenStream m_token_stream;
  ast::model::Class *m_root{nullptr};
  bool m_verbose;
  AstMap m_ast;

  template <typename T>
  std::unique_ptr<T> getAst(antlr4::ParserRuleContext *ctx) {
    auto iter = m_ast.find(ctx);
    assert(iter != m_ast.end());
    std::unique_ptr<T> val = static_unique_ptr_cast<T>(move(iter->second));
    assert(val != nullptr);
    return val;
  }

  void setAst(antlr4::ParserRuleContext *ctx,
              std::unique_ptr<ast::INode> node) {
    auto iter = m_ast.find(ctx);
    (void)iter;  // avoid unused warning when assertions removed
    assert(node != nullptr);
    assert(iter == m_ast.end());
    m_ast[ctx] = std::move(node);
  }

  void linkAst(antlr4::ParserRuleContext *to, antlr4::ParserRuleContext *from) {
    auto iter = m_ast.find(from);
    assert(iter != m_ast.end());
    setAst(to, std::move(iter->second));
  }

  /**
   * Helper methods
   */

  std::string indent(int n);

  /**
   * Listener Functions
   */
 public:
  void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override;
  void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override;
  void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
  void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
  void exitExpr_number(ModelicaParser::Expr_numberContext * /*ctx*/) override;
  void exitClass_definition(
      ModelicaParser::Class_definitionContext * /*ctx*/) override;
  // void exitExpr_simple(ModelicaParser::Expr_simpleContext * /*ctx*/)
  // override;
};

}  // namespace cymoca

#endif
