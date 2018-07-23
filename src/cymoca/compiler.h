#ifndef CYMOCA_COMPILER_H_
#define CYMOCA_COMPILER_H_

#include <any>
#include <memory>
#include <ostream>
#include <unordered_map>

#include "ast/ast.h"
#include "util.h"

#include "modelica_antlr/ModelicaBaseListener.h"
#include "modelica_antlr/ModelicaLexer.h"

using namespace modelica_antlr;

namespace cymoca {

class CompilerException : public std::runtime_error {
  std::string msg{};

 public:
  CompilerException(const std::string &arg, const char *file, int line)
      : std::runtime_error(arg) {
    std::ostringstream o;
    o << file << ":" << line << ": " << arg;
    msg = o.str();
  }
  ~CompilerException() throw() {}
  const char *what() const throw() { return msg.c_str(); }
};
#define compiler_exception(arg) CompilerException(arg, __FILE__, __LINE__);

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
  using AstMap = std::unordered_map<antlr4::ParserRuleContext *, std::any>;

 protected:
  std::unique_ptr<ModelicaParser> m_parser;
  antlr4::ANTLRInputStream m_input;
  ModelicaLexer m_lexer;
  antlr4::CommonTokenStream m_token_stream;
  ast::model::Class *m_root{nullptr};
  bool m_verbose;
  AstMap m_ast;

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

  // model

  // conditions
  void exitCond_bool(ModelicaParser::Cond_boolContext *ctx) override;
  void exitCond_binary(ModelicaParser::Cond_binaryContext *ctx) override;
  void exitCond_unary(ModelicaParser::Cond_unaryContext *ctx) override;
  void exitCond_compare(ModelicaParser::Cond_compareContext *ctx) override;
  void exitCond_func(ModelicaParser::Cond_funcContext *ctx) override;
  void exitCond_ref(ModelicaParser::Cond_refContext *ctx) override;

  // equations
  void exitEq_simple(ModelicaParser::Eq_simpleContext *ctx) override;
  void exitEq_block(ModelicaParser::Eq_blockContext *ctx) override;
  void exitEq_if(ModelicaParser::Eq_ifContext *ctx) override;
  void exitEq_for(ModelicaParser::Eq_forContext *ctx) override;
  void exitEq_connect(ModelicaParser::Eq_connectContext *ctx) override;
  void exitEq_when(ModelicaParser::Eq_whenContext *ctx) override;
  void exitEq_func(ModelicaParser::Eq_funcContext *ctx) override;

  // statements
  void exitStmt_ref(ModelicaParser::Stmt_refContext *ctx) override;
  void exitStmt_func(ModelicaParser::Stmt_funcContext *ctx) override;
  void exitStmt_key(ModelicaParser::Stmt_keyContext *ctx) override;
  void exitStmt_if(ModelicaParser::Stmt_ifContext *ctx) override;
  void exitStmt_for(ModelicaParser::Stmt_forContext *ctx) override;
  void exitStmt_while(ModelicaParser::Stmt_whileContext *ctx) override;
  void exitStmt_when(ModelicaParser::Stmt_whenContext *ctx) override;
  void exitFor_indices(ModelicaParser::For_indicesContext *ctx) override;
  void exitFor_index(ModelicaParser::For_indexContext *ctx) override;

  // expressions
  void exitExpr_number(ModelicaParser::Expr_numberContext *ctx) override;
  void exitExpr_ref(ModelicaParser::Expr_refContext *ctx) override;
  void exitExpr_simple(ModelicaParser::Expr_simpleContext *ctx) override;
  void exitExpr_if(ModelicaParser::Expr_ifContext *ctx) override;
  void exitExpr_func(ModelicaParser::Expr_funcContext *ctx) override;
  void exitExpr_string(ModelicaParser::Expr_stringContext *ctx) override;
  void exitExpr_range(ModelicaParser::Expr_rangeContext *ctx) override;
  void exitExpr_unary(ModelicaParser::Expr_unaryContext *ctx) override;
  void exitExpr_binary(ModelicaParser::Expr_binaryContext *ctx) override;
  void exitExpr_output(ModelicaParser::Expr_outputContext *ctx) override;
  void exitExpr_list(ModelicaParser::Expr_listContext *ctx) override;
  void exitExpr_end(ModelicaParser::Expr_endContext *ctx) override;
  void exitExpr_array(ModelicaParser::Expr_arrayContext *ctx) override;

  // function arguments
  void exitFunction_call_args(
      ModelicaParser::Function_call_argsContext *ctx) override;
  void exitFunc_arg_expr(ModelicaParser::Func_arg_exprContext *ctx) override;
  void exitFunc_arg_for(ModelicaParser::Func_arg_forContext *ctx) override;
  void exitFunc_arg_func(ModelicaParser::Func_arg_funcContext *ctx) override;
  void exitFunc_arg_named(ModelicaParser::Func_arg_namedContext *ctx) override;

  // elements
  void exitElem_import(ModelicaParser::Elem_importContext *ctx) override;
  void exitElem_extends(ModelicaParser::Elem_extendsContext *ctx) override;
  void exitElem_class(ModelicaParser::Elem_classContext *ctx) override;
  void exitElem_comp(ModelicaParser::Elem_compContext *ctx) override;

  // misc
  void exitClass_prefixes(ModelicaParser::Class_prefixesContext *ctx) override;
  void exitStored_definition(
      ModelicaParser::Stored_definitionContext *ctx) override;
  void exitEnumeration_literal(
      ModelicaParser::Enumeration_literalContext *ctx) override;
  void exitComposition(ModelicaParser::CompositionContext *ctx) override;
  void exitExternal_function_call(
      ModelicaParser::External_function_callContext *ctx) override;
  void exitConstraining_clause(
      ModelicaParser::Constraining_clauseContext *ctx) override;
  void exitComponent_clause(
      ModelicaParser::Component_clauseContext *ctx) override;
  void exitType_prefix(ModelicaParser::Type_prefixContext *ctx) override;
  void exitComponent_declaration(
      ModelicaParser::Component_declarationContext *ctx) override;
  void exitClass_modification(
      ModelicaParser::Class_modificationContext *ctx) override;
  void exitStmt_block(ModelicaParser::Stmt_blockContext *ctx) override;
  void exitEquation_section(
      ModelicaParser::Equation_sectionContext *ctx) override;
  void exitAlgorithm_section(
      ModelicaParser::Algorithm_sectionContext *ctx) override;
  void exitType_specifier(ModelicaParser::Type_specifierContext *ctx) override;
  void exitName(ModelicaParser::NameContext *ctx) override;
  void exitComponent_reference(
      ModelicaParser::Component_referenceContext *ctx) override;
  void exitArray_arg_expr(ModelicaParser::Array_arg_exprContext *ctx) override;
  void exitArray_arg_for(ModelicaParser::Array_arg_forContext *ctx) override;
  void exitArray_arguments(
      ModelicaParser::Array_argumentsContext *ctx) override;
  void exitNamed_arguments(
      ModelicaParser::Named_argumentsContext *ctx) override;
  void exitNamed_argument(ModelicaParser::Named_argumentContext *ctx) override;
  void exitOutput_expression_list(
      ModelicaParser::Output_expression_listContext *ctx) override;
  void exitExpression_list(
      ModelicaParser::Expression_listContext *ctx) override;
  void exitArray_subscripts(
      ModelicaParser::Array_subscriptsContext *ctx) override;
  void exitSubscript(ModelicaParser::SubscriptContext *ctx) override;
  void exitComment(ModelicaParser::CommentContext *ctx) override;
  void exitString_comment(ModelicaParser::String_commentContext *ctx) override;
  void exitAnnotation(ModelicaParser::AnnotationContext *ctx) override;
  void exitClass_def_long(ModelicaParser::Class_def_longContext *ctxt) override;
  void exitClass_def_der(ModelicaParser::Class_def_derContext *ctxt) override;
  void exitClass_def_short(
      ModelicaParser::Class_def_shortContext *ctxt) override;
  void exitClass_def_type(ModelicaParser::Class_def_typeContext *ctxt) override;
  void exitClass_def_enum(ModelicaParser::Class_def_enumContext *ctxt) override;
  void exitArg_modification(
      ModelicaParser::Arg_modificationContext *ctxt) override;
  void exitArg_redeclare_class(
      ModelicaParser::Arg_redeclare_classContext *ctxt) override;
  void exitArg_redeclare_element(
      ModelicaParser::Arg_redeclare_elementContext *ctxt) override;
  void exitModification_class(
      ModelicaParser::Modification_classContext *ctx) override;
  void exitModification_equation(
      ModelicaParser::Modification_equationContext *ctx) override;
  void exitModification_statement(
      ModelicaParser::Modification_statementContext *ctx) override;
};

}  // namespace cymoca

#endif
