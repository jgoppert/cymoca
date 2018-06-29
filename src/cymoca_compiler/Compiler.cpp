//
// Created by jgoppert on 3/17/18.
//

#include "Compiler.h"
#include "ast/ast.h"


namespace cymoca {

Compiler::Compiler(ifstream &text) :
    ModelicaBaseListener(),
    _parser(nullptr),
    _input(text),
    _lexer(&_input),
    _tokenStream(&_lexer),
    _root(nullptr),
    _verbose(false),
    _ast() {
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

void Compiler::visitErrorNode(antlr4::tree::ErrorNode *node) {
}

void Compiler::enterEveryRule(antlr4::ParserRuleContext *ctx) {
  if (_verbose) {
    cout << indent(ctx->depth()) << ">> " << _parser->getRuleNames()[ctx->getRuleIndex()] << endl;
  }
}

void Compiler::exitEveryRule(antlr4::ParserRuleContext *ctx) {
  if (_verbose) {
    cout << indent(ctx->depth()) << "<< " << _parser->getRuleNames()[ctx->getRuleIndex()] << endl;
  }
}

void Compiler::exitPrimary_unsigned_number(ModelicaParser::Primary_unsigned_numberContext *ctx) {
  stringstream ss(ctx->getText());
  double num;
  ss >> num;
  _ast[ctx] = make_unique<ast::Number>(num);
}

void Compiler::exitExpr_negative(ModelicaParser::Expr_negativeContext *ctx) {
  auto e = ast<ast::Expr>(ctx->expr());
  _ast[ctx] = make_unique<ast::UnaryExpr>(ast::UnaryOp::NEG, move(e));
}

void Compiler::exitPrimary_component_reference(ModelicaParser::Primary_component_referenceContext *ctx) {
  // TODO handle mutliple levels of naming in IDENT
  _ast[ctx] = make_unique<ast::ComponentRef>(ctx->component_reference()->getText());
}

void Compiler::exitComposition(ModelicaParser::CompositionContext *ctx) {
  // component declarations
  // TODO handle more than one element list
  auto newComp = make_unique<ast::ComponentDict>();
  for (auto & elemList: ctx->element_list()) {
    for (auto & elem: elemList->element()) {
      auto d = ast<ast::ComponentDict>(elem);
      for (auto &key_val: d->memory()) {
        newComp->set(key_val.first, move(key_val.second));
      }
    }
  }

  // equations
  auto newEq = make_unique<ast::EquationList>();
  for (auto &eq_sec: ctx->equation_section()) {
    auto sec = ast<ast::EquationList>(eq_sec->equation_list());
    for (auto &eq: sec->memory()) {
      newEq->append(move(eq));
    }
  }

  //for (auto &alg_sec: ctx->algorithm_section()) {
  //  for (auto &stmt: alg_sec->statement_list()->statement()) {
  //  }
  //}

  auto c = make_unique<ast::Class>(move(newComp), move(newEq));
  _root = c.get();
  ast(ctx, move(c));
}

void Compiler::exitExpression_simple(ModelicaParser::Expression_simpleContext *ctx) {
  linkAst(ctx, ctx->simple_expression());
}

void Compiler::exitSimple_expression(ModelicaParser::Simple_expressionContext *ctx) {
  // TODO handle other : expr's
  linkAst(ctx, ctx->expr(0));
}

void Compiler::exitEquation_simple(ModelicaParser::Equation_simpleContext *ctx) {
  auto left = ast<ast::Expr>(ctx->simple_expression());
  auto right = ast<ast::Expr>(ctx->expression());
  ast(ctx, make_unique<ast::SimpleEquation>(move(left), move(right)));
}

void Compiler::exitWhen_equation(ModelicaParser::When_equationContext *ctx) {
  auto whenEq = make_unique<ast::WhenEquation>();
  for (size_t i = 0; i < ctx->equation_list().size(); i++) {
    auto eqList = ast<ast::EquationList>(ctx->equation_list(i));
    if (i < ctx->expression().size()) {
      auto cond = ast<ast::LogicExpr>(ctx->expression(i));
      whenEq->append(make_unique<ast::EquationBlock >(move(cond), move(eqList)));
    } else {
      whenEq->append(make_unique<ast::EquationBlock >(
          make_unique<ast::Boolean>(true), move(eqList)));
    }
  }
  ast(ctx, move(whenEq));
}

void Compiler::exitEquation_list(ModelicaParser::Equation_listContext *ctx) {
  auto eqList = make_unique<ast::EquationList>();
  for (auto &eq: ctx->equation()) {
    // need to avoid deleting original
    auto eqVal = ast<ast::Equation>(eq);
    eqList->append(move(eqVal));
  }
  ast(ctx, move(eqList));
}

void Compiler::exitPrimary_der(ModelicaParser::Primary_derContext *ctx) {
  auto var = ast<ast::Expr>(ctx->function_call_args()->function_arguments());
  auto args = make_unique<ast::Args>();
  args->append(move(var));
  unique_ptr<ast::Node> a = make_unique<ast::FunctionCall>("der", move(args));
  ast(ctx, move(a));
}

void Compiler::exitArgs_expression(ModelicaParser::Args_expressionContext *ctx) {
  linkAst(ctx, ctx->expression());
}

void Compiler::exitExpr_relation(ModelicaParser::Expr_relationContext *ctx) {
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

void Compiler::exitPrimary_output_expression_list(ModelicaParser::Primary_output_expression_listContext *ctx) {
  linkAst(ctx, ctx->output_expression_list());
}

void Compiler::exitOutput_expression_list(ModelicaParser::Output_expression_listContext *ctx) {
  // TODO handle other items in list
  linkAst(ctx, ctx->expression(0));
}

void Compiler::exitEquation(ModelicaParser::EquationContext *ctx) {
  linkAst(ctx, ctx->equation_options());
}

void Compiler::exitEquation_options(ModelicaParser::Equation_optionsContext *ctx) {
  linkAst(ctx, static_cast<antlr4::ParserRuleContext *>(ctx->children[0]));
}

void Compiler::exitStatement_options(ModelicaParser::Statement_optionsContext *ctx) {
  linkAst(ctx, static_cast<antlr4::ParserRuleContext *>(ctx->children[0]));
}

void Compiler::exitIf_equation(ModelicaParser::If_equationContext *ctx) {
  auto ifEq = make_unique<ast::IfEquation >();
  for (size_t i = 0; i < ctx->equation_list().size(); i++) {
    auto eqList = ast<ast::EquationList>(ctx->equation_list(i));
    if (i < ctx->expression().size()) {
      auto cond = ast<ast::LogicExpr>(ctx->expression(i));
      ifEq->append(make_unique<ast::EquationBlock >(move(cond), move(eqList)));
    } else {
      ifEq->append(make_unique<ast::EquationBlock >(
          make_unique<ast::Boolean>(true), move(eqList)));
    }
  }
  ast(ctx, move(ifEq));
}

void Compiler::exitElement_component_definition(ModelicaParser::Element_component_definitionContext *ctx) {
  auto dict = make_unique<ast::ComponentDict>();
  auto type = ctx->component_clause()->type_specifier()->getText();
  auto prefixStr = ctx->component_clause()->type_prefix()->getText();
  for (auto comp: ctx->component_clause()->component_list()->component_declaration()) {
    auto name = comp->declaration()->IDENT()->getText();
    ast::Prefix prefix;
    if (prefixStr.compare("parameter") == 0) {
      prefix = ast::Prefix::PARAMETER;
    } else if (prefixStr.compare("constant") == 0) {
      prefix = ast::Prefix::CONSTANT;
    } else if (prefixStr.compare("") == 0) {
      prefix = ast::Prefix::VARIABLE;
    } else {
      assert(false);
    }
    auto c = make_unique<ast::Component>(name, type, prefix);
    dict->set(name, move(c));
  }
  ast(ctx, move(dict));
}

} // cymoca
