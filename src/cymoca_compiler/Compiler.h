//
// Created by jgoppert on 3/17/18.
//

#ifndef CYMOCA_COMPILER_H
#define CYMOCA_COMPILER_H

#include <modelica_antlr/ModelicaBaseListener.h>
#include <modelica_xsd/Modelica.hxx>

using namespace modelica_antlr;
using namespace antlr4;

namespace cymoca {

class Compiler : public ModelicaBaseListener {
 private:
  Parser *_parser;
  std::map<ParserRuleContext *, ::xml_schema::Type *> _ast;
  Modelica *_astResult;
 public:
  void compile(std::ifstream & text);
  Compiler();
  const Modelica &getAst();
  void printXML(std::ostream & out);
  void exitStored_definition(ModelicaParser::Stored_definitionContext *ctx) override;
  void exitStored_definition_class(ModelicaParser::Stored_definition_classContext *ctx) override;
  void exitClass_definition(ModelicaParser::Class_definitionContext *ctx) override;
  void exitClass_spec_comp(ModelicaParser::Class_spec_compContext *ctx) override;
};

} // cymoca

#endif //CYMOCA_COMPILER_H
