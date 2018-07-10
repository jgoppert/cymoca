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

  /**
   * This function gets a pointer to the AST
   * associated with ctx.
   */
  template <typename T>
  T *getAst(antlr4::ParserRuleContext *ctx) {
    auto iter = m_ast.find(ctx);
    assert(iter != m_ast.end());
    auto p = dynamic_cast<T *>(iter->second.get());
    assert(p);
    return p;
  }

  /**
   * This function gets the AST associated with ctx and
   * transfers ownership to the caller, removing it from
   * the ast dictionary.
   */
  template <typename T>
  std::unique_ptr<T> moveAst(antlr4::ParserRuleContext *ctx) {
    auto iter = m_ast.find(ctx);
    assert(iter != m_ast.end());
    auto p = static_unique_ptr_cast<T>(std::move(iter->second));
    assert(p);
    return p;
  }

  /**
   * This function gets the AST associated with ctx and
   * makes a copy which is returns to the caller.
   */
  template <typename T>
  std::unique_ptr<T> cloneAst(antlr4::ParserRuleContext *ctx) {
    auto iter = m_ast.find(ctx);
    assert(iter != m_ast.end());
    return iter->second->cloneAs<T>();
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
  void exitExpr_ref(ModelicaParser::Expr_refContext * /*ctx*/) override;
  void exitClass_definition(
      ModelicaParser::Class_definitionContext * /*ctx*/) override;
  // void exitExpr_simple(ModelicaParser::Expr_simpleContext * /*ctx*/)
  // override;
  void exitCond_bool(ModelicaParser::Cond_boolContext * /*ctx*/) override;
  void exitCond_binary(ModelicaParser::Cond_binaryContext * /*ctx*/) override;
};

}  // namespace cymoca

#endif
