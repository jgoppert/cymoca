//
// Created by jgoppert on 3/17/18.
//

#include "Compiler.h"

namespace cymoca {

class LispListener : public ast::Listener {
 public:
  LispListener() : _ss() {
  }
  void enter(ast::UnsignedNumber *ctx) override {
    _ss << ctx->val();
  }
  void enter(ast::Negative *ctx) override {
    _ss << "-";
  }
  void enter(ast::ComponentRef *ctx) override {
    _ss << ctx->name();
  }
  void enterEvery(ast::Node *ctx) override {
    _ss << "(";
  }
  void exitEvery(ast::Node *ctx) override {
    _ss << ")";
  }
  std::string get() {
    return _ss.str();
  }
 private:
  std::stringstream _ss;
};

Compiler::Compiler(std::ifstream &text) :
    _parser(nullptr),
    _input(text),
    _lexer(&_input),
    _tokenStream(&_lexer),
    _root(nullptr) {
  _tokenStream.fill();
  _parser = std::make_shared<ModelicaParser>(&_tokenStream);
  antlr4::tree::ParseTree *tree = _parser->stored_definition();
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
  //std::cout << "terminal: " << node->getText() << std::endl;
}

void Compiler::visitErrorNode(antlr4::tree::ErrorNode *node) {
}

void Compiler::enterEveryRule(antlr4::ParserRuleContext *ctx) {
  //std::cout << indent(ctx->depth()) << ">> " << _parser->getRuleNames()[ctx->getRuleIndex()] << std::endl;
}

void Compiler::exitEveryRule(antlr4::ParserRuleContext *ctx) {
  //std::cout << indent(ctx->depth()) << "<< " << _parser->getRuleNames()[ctx->getRuleIndex()] << std::endl;
}

void Compiler::exitPrimary_unsigned_number(ModelicaParser::Primary_unsigned_numberContext *ctx) {
  std::stringstream ss(ctx->getText());
  double num;
  ss >> num;
  _ast[ctx] = std::make_shared<ast::UnsignedNumber>(num);
}

void Compiler::exitExpr_negative(ModelicaParser::Expr_negativeContext *ctx) {
  auto f = _ast.find(ctx->expr());
  if (f == _ast.end()) {
    throw std::runtime_error("failed to find expression");
  }
  _ast[ctx] = std::make_shared<ast::Negative>(f->second);
  LispListener listener;
  ast::Walker walker;
  walker.walk(&listener, _ast[ctx].get());
  std::cout << listener.get() << std::endl;
}

void Compiler::exitPrimary_compoment_reference(ModelicaParser::Primary_compoment_referenceContext *ctx) {
  // TODO handle mutliple levels of naming in IDENT
  _ast[ctx] = std::make_shared<ast::ComponentRef>(ctx->component_reference()->getText());
}

void Compiler::exitComponent_declaration(ModelicaParser::Component_declarationContext *ctx) {
  _ast[ctx] = std::make_shared<ast::Component>(ctx->declaration()->getText());
}

void Compiler::exitComposition(ModelicaParser::CompositionContext *ctx) {
  ast::Class c;

  for (auto elem_list: ctx->element_list()) {
    for (auto elem: elem_list->element()) {
    }
  }

  for (auto eq_sec: ctx->equation_section()) {
    for (auto eq: eq_sec->equation_list()->equation()) {
    }
  }

  for (auto alg_sec: ctx->algorithm_section()) {
    for (auto stmt: alg_sec->statement_list()->statement()) {
    }
  }
}

void Compiler::exitExpression_simple(ModelicaParser::Expression_simpleContext *ctx) {
  _ast[ctx] = _ast[ctx->simple_expression()];
}

void Compiler::exitSimple_expression(ModelicaParser::Simple_expressionContext *ctx) {
  // TODO handle other : expr's
  _ast[ctx] = _ast[ctx->expr(0)];
}

void Compiler::exitEquation_simple(ModelicaParser::Equation_simpleContext *ctx) {
  _ast[ctx] = std::make_shared<ast::Equation>(
      _ast[ctx->simple_expression()].get(),
      _ast[ctx->expression()].get());
}

} // cymoca
