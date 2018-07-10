//
// Created by jgoppert on 3/17/18.
//

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



void Compiler::exitClass_definition(
    ModelicaParser::Class_definitionContext *ctx) {
  auto cls = std::make_unique<ast::model::Class>(
      std::make_unique<ast::model::ElementDict>(),
      std::make_unique<ast::equation::List>());
  setAst(ctx, std::move(cls));
  std::cout << "exit class def" << std::endl;
  m_root = getAst<ast::model::Class>(ctx);
}

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

//-----------------------------------------------------------------------------
// equations
//-----------------------------------------------------------------------------

void Compiler::exitEq_simple(ModelicaParser::Eq_simpleContext *ctx) {
  // auto left = cloneAst<ast::expression::Base>(ctx->expr());
  // auto right = cloneAst<ast::expression::Base>(ctx->expression());
  // setAst(ctx, std::make_unique<ast::equation::Simple>(
  //         std::move(left), std::move(right)));
}

void Compiler::exitEq_block(ModelicaParser::Eq_blockContext *ctx) {}

void Compiler::exitEq_if(ModelicaParser::Eq_ifContext *ctx) {}
void Compiler::exitEq_for(ModelicaParser::Eq_forContext *ctx) {}
void Compiler::exitEq_connect(ModelicaParser::Eq_connectContext *ctx) {}
void Compiler::exitEq_when(ModelicaParser::Eq_whenContext *ctx) {}
void Compiler::exitEq_func(ModelicaParser::Eq_funcContext *ctx) {}

//-----------------------------------------------------------------------------
// statements
//-----------------------------------------------------------------------------

void Compiler::exitStmt_ref(ModelicaParser::Stmt_refContext *ctx) {}
void Compiler::exitStmt_func(ModelicaParser::Stmt_funcContext *ctx) {}
void Compiler::exitStmt_key(ModelicaParser::Stmt_keyContext *ctx) {}
void Compiler::exitStmt_if(ModelicaParser::Stmt_ifContext *ctx) {}
void Compiler::exitStmt_for(ModelicaParser::Stmt_forContext *ctx) {}
void Compiler::exitStmt_while(ModelicaParser::Stmt_whileContext *ctx) {}
void Compiler::exitStmt_when(ModelicaParser::Stmt_whenContext *ctx) {}
void Compiler::exitFor_indices(ModelicaParser::For_indicesContext *ctx) {}
void Compiler::exitFor_index(ModelicaParser::For_indexContext *ctx) {}

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

void Compiler::exitExpr_simple(ModelicaParser::Expr_simpleContext *ctx) {}
void Compiler::exitExpr_if(ModelicaParser::Expr_ifContext *ctx) {}
void Compiler::exitExpr_func(ModelicaParser::Expr_funcContext *ctx) {}
void Compiler::exitExpr_string(ModelicaParser::Expr_stringContext *ctx) {}
void Compiler::exitExpr_range(ModelicaParser::Expr_rangeContext *ctx) {}
void Compiler::exitExpr_unary(ModelicaParser::Expr_unaryContext *ctx) {}
void Compiler::exitExpr_binary(ModelicaParser::Expr_binaryContext *ctx) {}
void Compiler::exitCond_compare(ModelicaParser::Cond_compareContext *ctx) {}
void Compiler::exitCond_func(ModelicaParser::Cond_funcContext *ctx) {}
void Compiler::exitCond_ref(ModelicaParser::Cond_refContext *ctx) {}

}  // namespace cymoca
