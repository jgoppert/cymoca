//
// Created by jgoppert on 3/17/18.
//

#include "Compiler.h"

#include <modelica_antlr/ModelicaLexer.h>

using namespace modelica_antlr;
using namespace antlr4;

namespace cymoca {

Compiler::Compiler() :
    _ast(),
    _astResult(nullptr) {
}

void Compiler::compile(std::ifstream & text) {
  ANTLRInputStream aText(text);
  ModelicaLexer lexer(&aText);
  CommonTokenStream tokens(&lexer);
  tokens.fill();
  for (auto token : tokens.getTokens()) {
    std::cout << token->toString() << std::endl;
  }
  ModelicaParser parser(&tokens);
  tree::ParseTree *tree = parser.stored_definition();
  std::cout << tree->toStringTree(&parser) << std::endl << std::endl;
  tree::ParseTreeWalker::DEFAULT.walk(this, tree);
}

const Modelica &Compiler::getAst() {
  return *_astResult;
}

void Compiler::printXML(std::ostream & out) {
  std::stringstream ss;
  xml_schema::NamespaceInfomap map;
  map[""].name = "";
  map[""].schema = "Modelica.xsd";
  modelica(out, getAst(), map);
}

//void Compiler::enterEveryRule(ParserRuleContext *ctx) {
  //std::cout << "enter" << ctx->toString(_parser) << std::endl;
//}

//void Compiler::exitEveryRule(ParserRuleContext *ctx) {
  //std::cout << "exit" << ctx->toString(_parser) << std::endl;
//}

void Compiler::exitStored_definition(ModelicaParser::Stored_definitionContext *ctx) {
  Modelica *m = new Modelica("1.0");
  // populate class definitions
  for (auto &d : ctx->stored_definition_class()) {
    auto found = _ast.find(d);
    if (found == _ast.end()) {
      std::cout << "no ast for class" << std::endl;
    } else {
      auto cdef = (ClassDefinition *) found->second;
      m->classDefinition(*cdef);
    }
  }
  _ast[ctx] = m;
  _astResult = m;
}

void Compiler::exitStored_definition_class(ModelicaParser::Stored_definition_classContext *ctx) {
  // use class definition in class definition, but append final if it is there
  auto d = dynamic_cast<ClassDefinition *>(_ast[ctx->class_definition()]);
  d->final(ctx->FINAL() != nullptr);
  _ast[ctx] = d;
}

void Compiler::exitClass_definition(ModelicaParser::Class_definitionContext *ctx) {
  // use class definition in class_specifier, but append prefixes and encapsulated
  // TODO encapsulated not in AST
  //bool encapsulated = ctx->ENCAPSULATED() != nullptr;
  std::string class_type = ctx->class_prefixes()->class_type()->getText();
  auto d = dynamic_cast<ClassDefinition *>(_ast[ctx->class_specifier()]);
  auto c = ClassContents(ctx->class_prefixes()->class_type()->getText());
  c.partial(ctx->class_prefixes()->PARTIAL() != nullptr);
  d->class_(c);
  _ast[ctx] = d;
}

void Compiler::exitClass_spec_comp(ModelicaParser::Class_spec_compContext *ctx) {
  std::string name = ctx->IDENT()[0]->getText();
  std::string name_end = ctx->IDENT()[1]->getText();
  if (name != name_end) throw std::runtime_error("class identifiers do not match");
  auto c = new ClassDefinition(name);
  c->comment(ctx->string_comment()->getText());
  _ast[ctx] = c;
}

} // cymoca