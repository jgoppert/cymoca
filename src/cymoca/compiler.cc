#include "compiler.h"

namespace cymoca {

Compiler::Compiler(std::ifstream &text)
    : ModelicaBaseListener(),
      m_parser(nullptr),
      m_input(text),
      m_lexer(&m_input),
      m_token_stream(&m_lexer),
      m_root(nullptr),
      m_verbose(false),
      m_ast() {
  m_token_stream.fill();
  m_parser = std::make_unique<ModelicaParser>(&m_token_stream);
  antlr4::tree::ParseTree *tree = m_parser->stored_definition();
  antlr4::tree::ParseTreeWalker::DEFAULT.walk(this, tree);
}

std::string Compiler::indent(int n) {
  std::string s;

  for (int i = 0; i < n; i++) {
    s.append(" ");
  }

  return s;
}

void Compiler::visitTerminal(antlr4::tree::TerminalNode *node) {
  if (m_verbose) {
    std::cout << "terminal: " << node->getText() << std::endl;
  }
}

void Compiler::visitErrorNode(antlr4::tree::ErrorNode * /*node*/) {}

void Compiler::enterEveryRule(antlr4::ParserRuleContext *ctx) {
  if (m_verbose) {
    std::cout << indent(ctx->depth()) << ">> "
              << m_parser->getRuleNames()[ctx->getRuleIndex()] << std::endl;
  }
}

void Compiler::exitEveryRule(antlr4::ParserRuleContext *ctx) {
  if (m_verbose) {
    std::cout << indent(ctx->depth()) << "<< "
              << m_parser->getRuleNames()[ctx->getRuleIndex()] << std::endl;
  }
}

//-----------------------------------------------------------------------------
// model
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// condition
//-----------------------------------------------------------------------------

void Compiler::exitCond_bool(ModelicaParser::Cond_boolContext *ctx) {
  auto res = std::make_unique<ast::condition::Boolean>(false);
  if (ctx->TRUE()) {
    res->setValue(true);
  } else if (ctx->FALSE()) {
    res->setValue(false);
  } else {
    assert(false);
  }
  setAst(ctx, std::move(res));
}

void Compiler::exitCond_binary(ModelicaParser::Cond_binaryContext *ctx) {
  std::string op = ctx->op->getText();
  auto left = cloneAst<ast::condition::Base>(ctx->condition(0));
  auto right = cloneAst<ast::condition::Base>(ctx->condition(1));
  if (op == "and") {
    setAst(ctx, std::make_unique<ast::condition::And>(std::move(left),
                                                      std::move(right)));
  } else if (op == "or") {
    setAst(ctx, std::make_unique<ast::condition::Or>(std::move(left),
                                                     std::move(right)));
  } else {
    assert(false);
  }
}

void Compiler::exitCond_unary(ModelicaParser::Cond_unaryContext *ctx) {
  assert(ctx->op->getText() == "-");
  auto e = cloneAst<ast::condition::Base>(ctx->condition());
  setAst(ctx, std::make_unique<ast::condition::Not>(std::move(e)));
}

void Compiler::exitCond_compare(ModelicaParser::Cond_compareContext *ctx) {
  auto left = cloneAst<ast::expression::Base>(ctx->expr(0));
  auto right = cloneAst<ast::expression::Base>(ctx->expr(1));
  std::string op = ctx->op->getText();
  if (op == "<") {
    setAst(ctx, std::make_unique<ast::condition::LessThan>(std::move(left),
                                                           std::move(right)));
  } else if (op == "<=") {
    setAst(ctx, std::make_unique<ast::condition::LessThanOrEqual>(
                    std::move(left), std::move(right)));
  } else if (op == ">") {
    setAst(ctx, std::make_unique<ast::condition::GreaterThan>(
                    std::move(left), std::move(right)));
  } else if (op == ">=") {
    setAst(ctx, std::make_unique<ast::condition::GreaterThanOrEqual>(
                    std::move(left), std::move(right)));
  } else if (op == "==") {
    setAst(ctx, std::make_unique<ast::condition::Equal>(std::move(left),
                                                        std::move(right)));
  } else if (op == "<>") {
    setAst(ctx, std::make_unique<ast::condition::NotEqual>(std::move(left),
                                                           std::move(right)));
  } else {
    throw compiler_exception(std::string("unhandled operator") + op);
  }
}
void Compiler::exitCond_func(ModelicaParser::Cond_funcContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitCond_ref(ModelicaParser::Cond_refContext *ctx) {
  throw compiler_exception("not implemented");
}

//-----------------------------------------------------------------------------
// equations
//-----------------------------------------------------------------------------

void Compiler::exitEq_simple(ModelicaParser::Eq_simpleContext *ctx) {
  auto left = cloneAst<ast::expression::Base>(ctx->expr());
  auto right = cloneAst<ast::expression::Base>(ctx->expression());
  setAst(ctx, std::make_unique<ast::equation::Simple>(std::move(left),
                                                      std::move(right)));
}

void Compiler::exitEq_block(ModelicaParser::Eq_blockContext *ctx) {
  auto res = std::make_unique<ast::equation::List>();
  for (auto eq : ctx->equation()) {
    res->append(cloneAst<ast::equation::Base>(eq));
  }
  setAst(ctx, std::move(res));
}

void Compiler::exitEq_if(ModelicaParser::Eq_ifContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitEq_for(ModelicaParser::Eq_forContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitEq_connect(ModelicaParser::Eq_connectContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitEq_when(ModelicaParser::Eq_whenContext *ctx) {
  assert(ctx->condition().size() == ctx->eq_block().size());
  auto when = std::make_unique<ast::equation::When>();
  for (size_t i = 0; i < ctx->condition().size(); i++) {
    auto block = std::make_unique<ast::equation::Block>(
        cloneAst<ast::condition::Base>(ctx->condition(i)),
        cloneAst<ast::equation::List>(ctx->eq_block(i)));
    when->append(std::move(block));
  }
  setAst(ctx, std::move(when));
}
void Compiler::exitEq_func(ModelicaParser::Eq_funcContext *ctx) {
  throw compiler_exception("not implemented");
}

//-----------------------------------------------------------------------------
// statements
//-----------------------------------------------------------------------------

void Compiler::exitStmt_ref(ModelicaParser::Stmt_refContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitStmt_func(ModelicaParser::Stmt_funcContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitStmt_key(ModelicaParser::Stmt_keyContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitStmt_if(ModelicaParser::Stmt_ifContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitStmt_for(ModelicaParser::Stmt_forContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitStmt_while(ModelicaParser::Stmt_whileContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitStmt_when(ModelicaParser::Stmt_whenContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitFor_indices(ModelicaParser::For_indicesContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitFor_index(ModelicaParser::For_indexContext *ctx) {
  throw compiler_exception("not implemented");
}

//-----------------------------------------------------------------------------
// expressions
//-----------------------------------------------------------------------------

void Compiler::exitExpr_number(ModelicaParser::Expr_numberContext *ctx) {
  std::stringstream ss(ctx->getText());
  double value = 0;
  ss >> value;
  auto num = std::make_unique<ast::expression::Number>(value);
  setAst(ctx, std::move(num));
}

void Compiler::exitExpr_ref(ModelicaParser::Expr_refContext *ctx) {
  std::vector<std::string> ids;
  for (auto id : ctx->component_reference()->IDENT()) {
    ids.push_back(id->getText());
  }
  // TODO handle more than one string
  setAst(ctx, std::make_unique<ast::expression::Reference>(ids[0]));
}

void Compiler::exitExpr_simple(ModelicaParser::Expr_simpleContext *ctx) {
  linkAst(ctx, ctx->expr());
}
void Compiler::exitExpr_if(ModelicaParser::Expr_ifContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpr_func(ModelicaParser::Expr_funcContext *ctx) {
  if (ctx->component_reference()) {
    setAst(ctx,
           std::make_unique<ast::expression::Function>(
               cloneAst<ast::expression::Reference>(ctx->component_reference()),
               cloneAst<ast::expression::List>(ctx->function_call_args())));
  } else if (ctx->func) {
    setAst(
        ctx,
        std::make_unique<ast::expression::Function>(
            std::make_unique<ast::expression::Reference>(ctx->func->getText()),
            cloneAst<ast::expression::List>(ctx->function_call_args())));
  } else {
    assert(false);
  }
}
void Compiler::exitExpr_string(ModelicaParser::Expr_stringContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpr_range(ModelicaParser::Expr_rangeContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpr_unary(ModelicaParser::Expr_unaryContext *ctx) {
  setAst(ctx, std::make_unique<ast::expression::Negative>(
                  cloneAst<ast::expression::Base>(ctx->expr())));
}
void Compiler::exitExpr_binary(ModelicaParser::Expr_binaryContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpr_output(ModelicaParser::Expr_outputContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpr_list(ModelicaParser::Expr_listContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpr_end(ModelicaParser::Expr_endContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpr_array(ModelicaParser::Expr_arrayContext *ctx) {
  throw compiler_exception("not implemented");
}

//-----------------------------------------------------------------------------
// function arguments
//-----------------------------------------------------------------------------

void Compiler::exitFunction_call_args(
    ModelicaParser::Function_call_argsContext *ctx) {
  auto args = std::make_unique<ast::expression::List>();
  for (auto arg_ctx : ctx->function_argument()) {
    args->append(cloneAst<ast::expression::Base>(arg_ctx));
  }
  setAst(ctx, std::move(args));
}
void Compiler::exitFunc_arg_expr(ModelicaParser::Func_arg_exprContext *ctx) {
  setAst(ctx, cloneAst<ast::expression::Base>(ctx->expression()));
}
void Compiler::exitFunc_arg_for(ModelicaParser::Func_arg_forContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitFunc_arg_func(ModelicaParser::Func_arg_funcContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitFunc_arg_named(ModelicaParser::Func_arg_namedContext *ctx) {
  throw compiler_exception("not implemented");
}

//-----------------------------------------------------------------------------
// elements
//-----------------------------------------------------------------------------

void Compiler::exitElem_import(ModelicaParser::Elem_importContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitElem_extends(ModelicaParser::Elem_extendsContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitElem_class(ModelicaParser::Elem_classContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitElem_comp(ModelicaParser::Elem_compContext *ctx) {
  linkAst(ctx, ctx->component_clause());
}

//-----------------------------------------------------------------------------
// misc
//-----------------------------------------------------------------------------

void Compiler::exitClass_prefixes(ModelicaParser::Class_prefixesContext *ctx) {
  // pass, just a string, let level above handle this
}

void Compiler::exitStored_definition(
    ModelicaParser::Stored_definitionContext *ctx) {
  // throw compiler_exception("not implemented");
  // TODO
}
void Compiler::exitEnumeration_literal(
    ModelicaParser::Enumeration_literalContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitComposition(ModelicaParser::CompositionContext *ctx) {
  auto c = std::make_unique<ast::model::Class>(
      std::make_unique<ast::model::ElementDict>(),
      std::make_unique<ast::equation::List>());

  // elements
  enum class Visibility { PUBLIC, PRIVATE, PROTECTED };
  for (auto elem_sect :
       {std::make_pair(Visibility::PUBLIC, ctx->public_elem),
        std::make_pair(Visibility::PRIVATE, ctx->private_elem),
        std::make_pair(Visibility::PROTECTED, ctx->protected_elem)}) {
    for (auto e_ctx : elem_sect.second) {
      // TODO set visibility for element
      auto edict = getAst<ast::model::ElementDict>(e_ctx);
      for (auto &key_val : edict->getMap()) {
        auto e = key_val.second->cloneAs<ast::element::Base>();
        c->getElements().set(key_val.first, std::move(e));
      }
    }
  }

  // equations
  for (auto eq_sec : ctx->equation_section()) {
    for (auto eq : eq_sec->equation()) {
      c->getEquations().append(cloneAst<ast::equation::Base>(eq));
    }
  }

  // statements
  for (auto algo_sec : ctx->algorithm_section()) {
    for (auto s_ctx : algo_sec->statement()) {
      auto s = cloneAst<ast::statement::Base>(s_ctx);
      c->getStatements().append(std::move(s));
    }
  }
  m_root = c.get();
  setAst(ctx, std::move(c));
}
void Compiler::exitExternal_function_call(
    ModelicaParser::External_function_callContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitConstraining_clause(
    ModelicaParser::Constraining_clauseContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitComponent_clause(
    ModelicaParser::Component_clauseContext *ctx) {
  // TODO handle multi-level names
  auto elements = std::make_unique<ast::model::ElementDict>();
  auto type = ctx->type_specifier()->getText();
  auto prefix_str = ctx->type_prefix()->getText();
  ast::element::Prefix prefix;
  if (prefix_str == "parameter") {
    prefix = ast::element::Prefix::PARAMETER;
  } else if (prefix_str == "constant") {
    prefix = ast::element::Prefix::CONSTANT;
  } else if (prefix_str == "discrete") {
    prefix = ast::element::Prefix::DISCRETE;
  } else if (prefix_str == "") {
    prefix = ast::element::Prefix::VARIABLE;
  } else {
    throw compiler_exception("unhandled type prefix " + type);
  }
  for (auto d : ctx->component_declaration()) {
    std::string name = d->IDENT()->getText();
    elements->set(
        name, std::make_unique<ast::element::Component>(name, type, prefix));
  }
  setAst(ctx, std::move(elements));
}
void Compiler::exitType_prefix(ModelicaParser::Type_prefixContext *ctx) {
  // pass, just text, let level above handle this
}
void Compiler::exitComponent_declaration(
    ModelicaParser::Component_declarationContext *ctx) {
  // pass, handle in component_clause
}
void Compiler::exitClass_modification(
    ModelicaParser::Class_modificationContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitStmt_block(ModelicaParser::Stmt_blockContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitEquation_section(
    ModelicaParser::Equation_sectionContext *ctx) {
  auto eqs = std::make_unique<ast::equation::List>();
  for (auto eq : ctx->equation()) {
    eqs->append(cloneAst<ast::equation::Base>(eq));
  }
  setAst(ctx, std::move(eqs));
}
void Compiler::exitAlgorithm_section(
    ModelicaParser::Algorithm_sectionContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitType_specifier(ModelicaParser::Type_specifierContext *ctx) {
  // pass, just strings, let level above handle this
}
void Compiler::exitName(ModelicaParser::NameContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitComponent_reference(
    ModelicaParser::Component_referenceContext *ctx) {
  // TODO, split names
  setAst(ctx, std::make_unique<ast::expression::Reference>(ctx->getText()));
}
void Compiler::exitArray_arg_expr(ModelicaParser::Array_arg_exprContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitArray_arg_for(ModelicaParser::Array_arg_forContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitArray_arguments(
    ModelicaParser::Array_argumentsContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitNamed_arguments(
    ModelicaParser::Named_argumentsContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitNamed_argument(ModelicaParser::Named_argumentContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitOutput_expression_list(
    ModelicaParser::Output_expression_listContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitExpression_list(
    ModelicaParser::Expression_listContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitArray_subscripts(
    ModelicaParser::Array_subscriptsContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitSubscript(ModelicaParser::SubscriptContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitComment(ModelicaParser::CommentContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitString_comment(ModelicaParser::String_commentContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitAnnotation(ModelicaParser::AnnotationContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitClass_def_long(ModelicaParser::Class_def_longContext *ctx) {
  // TODO
  // throw compiler_exception("not implemented");
}
void Compiler::exitClass_def_der(ModelicaParser::Class_def_derContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitClass_def_short(
    ModelicaParser::Class_def_shortContext *ctxt) {
  throw compiler_exception("not implemented");
}
void Compiler::exitClass_def_type(ModelicaParser::Class_def_typeContext *ctxt) {
  throw compiler_exception("not implemented");
}
void Compiler::exitClass_def_enum(ModelicaParser::Class_def_enumContext *ctxt) {
  throw compiler_exception("not implemented");
}
void Compiler::exitArg_modification(
    ModelicaParser::Arg_modificationContext *ctxt) {
  throw compiler_exception("not implemented");
}
void Compiler::exitArg_redeclare_class(
    ModelicaParser::Arg_redeclare_classContext *ctxt) {
  throw compiler_exception("not implemented");
}
void Compiler::exitArg_redeclare_element(
    ModelicaParser::Arg_redeclare_elementContext *ctxt) {
  throw compiler_exception("not implemented");
}
void Compiler::exitModification_class(
    ModelicaParser::Modification_classContext *ctx) {
  throw compiler_exception("not implemented");
}
void Compiler::exitModification_equation(
    ModelicaParser::Modification_equationContext *ctx) {
  auto e = cloneAst<ast::expression::Base>(ctx->expression());
  // TODO
}
void Compiler::exitModification_statement(
    ModelicaParser::Modification_statementContext *ctx) {
  auto e = cloneAst<ast::expression::Base>(ctx->expression());
  // throw compiler_exception("not implemented");
}

}  // namespace cymoca
