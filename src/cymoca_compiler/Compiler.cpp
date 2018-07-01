//
// Created by jgoppert on 3/17/18.
//

#include "Compiler.h"

namespace cymoca {

Compiler::Compiler(ifstream &text)
    : ModelicaBaseListener(), _parser(nullptr), _input(text), _lexer(&_input),
      _tokenStream(&_lexer), _root(nullptr), _verbose(false), _ast() {
  _tokenStream.fill();
  _parser = make_unique<ModelicaParser>(&_tokenStream);
  antlr4::tree::ParseTree *tree = _parser->stored_definition();
  antlr4::tree::ParseTreeWalker::DEFAULT.walk(this, tree);
}

string Compiler::indent(int n) {
  string s;

  for (int i = 0; i < n; i++) {
    s.append(" ");
  }

  return s;
}

void Compiler::visitTerminal(antlr4::tree::TerminalNode *node) {
  if (_verbose) {
    cout << "terminal: " << node->getText() << endl;
  }
}

void Compiler::visitErrorNode(antlr4::tree::ErrorNode *node) {}

void Compiler::enterEveryRule(antlr4::ParserRuleContext *ctx) {
  if (_verbose) {
    cout << indent(ctx->depth()) << ">> "
         << _parser->getRuleNames()[ctx->getRuleIndex()] << endl;
  }
}

void Compiler::exitEveryRule(antlr4::ParserRuleContext *ctx) {
  if (_verbose) {
    cout << indent(ctx->depth()) << "<< "
         << _parser->getRuleNames()[ctx->getRuleIndex()] << endl;
  }
}

void Compiler::exitComposition(ModelicaParser::CompositionContext *ctx) {
  // component declarations
  auto newComp = make_unique<ast::ComponentDict>();
  // TODO store private/public/protected access
  for (auto &elem : ctx->private_elem) {
    auto d = ast<ast::ComponentDict>(elem);
    for (auto &key_val : d->memory()) {
      newComp->set(key_val.first, move(key_val.second));
    }
  }
  for (auto &elem : ctx->public_elem) {
    auto d = ast<ast::ComponentDict>(elem);
    for (auto &key_val : d->memory()) {
      newComp->set(key_val.first, move(key_val.second));
    }
  }
  for (auto &elem : ctx->protected_elem) {
    auto d = ast<ast::ComponentDict>(elem);
    for (auto &key_val : d->memory()) {
      newComp->set(key_val.first, move(key_val.second));
    }
  }

  // equations
  auto newEq = make_unique<ast::EquationList>();

  for (auto &eq_sec : ctx->equation_section()) {
    for (auto &eq : eq_sec->equation()) {
      newEq->append(ast<ast::Equation>(eq));
    }
  }

  // for (auto &alg_sec: ctx->algorithm_section()) {
  //  for (auto &stmt: alg_sec->statement_list()->statement()) {
  //  }
  //}

  auto c = make_unique<ast::Class>(move(newComp), move(newEq));
  _root = c.get();
  ast(ctx, move(c));
}

void Compiler::exitExpression_simple(
    ModelicaParser::Expression_simpleContext *ctx) {
  linkAst(ctx, ctx->expr(0));
}

void Compiler::exitElem_comp(ModelicaParser::Elem_compContext *ctx) {
  auto dict = make_unique<ast::ComponentDict>();
  auto type = ctx->component_clause()->type_specifier()->getText();
  auto prefixStr = ctx->component_clause()->type_prefix()->getText();

  for (auto comp :
       ctx->component_clause()->component_list()->component_declaration()) {
    auto name = comp->IDENT()->getText();
    ast::Prefix prefix;

    if (prefixStr == "parameter") {
      prefix = ast::Prefix::PARAMETER;

    } else if (prefixStr == "constant") {
      prefix = ast::Prefix::CONSTANT;

    } else if (prefixStr.empty()) {
      prefix = ast::Prefix::VARIABLE;

    } else {
      assert(false);
    }

    auto c = make_unique<ast::Component>(name, type, prefix);
    dict->set(name, move(c));
  }

  ast(ctx, move(dict));
}

void Compiler::exitExpr_number(ModelicaParser::Expr_numberContext *ctx) {
  stringstream ss(ctx->getText());
  double num;
  ss >> num;
  ast(ctx, make_unique<ast::Number>(num));
}

void Compiler::exitExpr_unary(ModelicaParser::Expr_unaryContext *ctx) {
  auto e = ast<ast::Expr>(ctx->expr());
  assert(ctx->op->getText() == "-");
  ast(ctx, make_unique<ast::UnaryExpr>(ast::UnaryOp::NEG, move(e)));
}

void Compiler::exitExpr_binary(ModelicaParser::Expr_binaryContext *ctx) {
  auto left = ast<ast::Expr>(ctx->expr(0));
  auto right = ast<ast::Expr>(ctx->expr(1));
  string op = ctx->op->getText();
  static unordered_map<string, ast::RelationOp> convertMap{
      {"<", ast::RelationOp::LT},
      {">", ast::RelationOp::GT},
      {"<=", ast::RelationOp::LE},
      {">=", ast::RelationOp::GE},
      {"<>", ast::RelationOp::NEQ}};
  auto iter = convertMap.find(op);
  assert(iter != convertMap.end());
  auto relOp = iter->second;
  ast(ctx, make_unique<ast::Relation>(move(left), relOp, move(right)));
}

void Compiler::exitExpr_ref(ModelicaParser::Expr_refContext *ctx) {
  // TODO handle mutliple levels of naming in IDENT
  ast(ctx,
      make_unique<ast::ComponentRef>(ctx->component_reference()->getText()));
}

void Compiler::exitExpr_func(ModelicaParser::Expr_funcContext *ctx) {
  auto var = ast<ast::Expr>(ctx->function_call_args()->function_arguments());
  auto args = make_unique<ast::Args>();
  args->append(move(var));
  if (ctx->component_reference())
    throw logic_error("not implemented");
  string name = ctx->func->getText();
  unique_ptr<ast::Node> a = make_unique<ast::FunctionCall>(name, move(args));
  ast(ctx, move(a));
}

void Compiler::exitExpr_output(ModelicaParser::Expr_outputContext *ctx) {
  linkAst(ctx, ctx->output_expression_list()->expression(0));
}

void Compiler::exitEq_simple(ModelicaParser::Eq_simpleContext *ctx) {
  auto left = ast<ast::Expr>(ctx->expr(0));
  auto right = ast<ast::Expr>(ctx->expression());
  ast(ctx, make_unique<ast::SimpleEquation>(move(left), move(right)));
}

void Compiler::exitEq_if(ModelicaParser::Eq_ifContext *ctx) {
  auto ifEq = make_unique<ast::IfEquation>();

  for (size_t i = 0; i < ctx->eq_block().size(); i++) {
    auto eqList = ast<ast::EquationList>(ctx->eq_block(i));

    if (i < ctx->expression().size()) {
      auto cond = ast<ast::LogicExpr>(ctx->expression(i));
      ifEq->append(make_unique<ast::EquationBlock>(move(cond), move(eqList)));

    } else {
      ifEq->append(make_unique<ast::EquationBlock>(
          make_unique<ast::Boolean>(true), move(eqList)));
    }
  }

  ast(ctx, move(ifEq));
}

void Compiler::exitEq_for(ModelicaParser::Eq_forContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitEq_connect(ModelicaParser::Eq_connectContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitEq_when(ModelicaParser::Eq_whenContext *ctx) {
  auto whenEq = make_unique<ast::WhenEquation>();

  for (size_t i = 0; i < ctx->eq_block().size(); i++) {
    auto eqList = ast<ast::EquationList>(ctx->eq_block(i));

    if (i < ctx->expression().size()) {
      auto cond = ast<ast::LogicExpr>(ctx->expression(i));
      whenEq->append(make_unique<ast::EquationBlock>(move(cond), move(eqList)));

    } else {
      whenEq->append(make_unique<ast::EquationBlock>(
          make_unique<ast::Boolean>(true), move(eqList)));
    }
  }
  ast(ctx, move(whenEq));
}

void Compiler::exitEq_func(ModelicaParser::Eq_funcContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitStmt_ref(ModelicaParser::Stmt_refContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitStmt_func(ModelicaParser::Stmt_funcContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitStmt_keyword(ModelicaParser::Stmt_keywordContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitStmt_if(ModelicaParser::Stmt_ifContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitStmt_for(ModelicaParser::Stmt_forContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitStmt_while(ModelicaParser::Stmt_whileContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitStmt_when(ModelicaParser::Stmt_whenContext *ctx) {
  throw logic_error("not implemented");
}
void Compiler::exitEq_block(ModelicaParser::Eq_blockContext *ctx) {
  auto eqList = make_unique<ast::EquationList>();

  for (auto &eq : ctx->equation()) {
    // need to avoid deleting original
    auto eqVal = ast<ast::Equation>(eq);
    eqList->append(move(eqVal));
  }

  ast(ctx, move(eqList));
}
void Compiler::exitStmt_block(ModelicaParser::Stmt_blockContext *ctx) {
  throw logic_error("not implemented");
}

void Compiler::exitArgs_expr(ModelicaParser::Args_exprContext *ctx) {
  linkAst(ctx, ctx->expression());
}

} // namespace cymoca

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
