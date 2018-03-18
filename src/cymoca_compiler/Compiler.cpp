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
    _root(nullptr) {
}
void Compiler::compile(std::ifstream &text) {
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
  Modelica * m = dynamic_cast<Modelica *>(_ast[_root]);
  assert(m != nullptr);
  return *m;
}
void Compiler::printXML(std::ostream &out) {
  std::stringstream ss;
  xml_schema::NamespaceInfomap map;
  map[""].name = "";
  map[""].schema = "Modelica.xsd";
  modelica(out, getAst(), map);
}
void Compiler::visitTerminal(tree::TerminalNode *node) {

}
void Compiler::visitErrorNode(tree::ErrorNode *node) {

}
void Compiler::enterEveryRule(ParserRuleContext *ctx) {

}
void Compiler::exitEveryRule(ParserRuleContext *ctx) {

}
void Compiler::enterStored_definition(ModelicaParser::Stored_definitionContext *ctx) {
}
void Compiler::exitStored_definition(ModelicaParser::Stored_definitionContext *ctx) {
  auto m = new Modelica("1.0");
  // populate class definitions
  for (auto &d: ctx->stored_definition_class()) {
    assert(d != nullptr);
    auto * c = dynamic_cast<ClassDefinition *>(_ast[d]);
    assert(c != nullptr);
    m->classDefinition(*c);
  }
  _root = ctx;
  _ast[ctx] = m;
}
void Compiler::enterStored_definition_class(ModelicaParser::Stored_definition_classContext *ctx) {

}
void Compiler::exitStored_definition_class(ModelicaParser::Stored_definition_classContext *ctx) {
  // use class definition in class definition, but append final if it is there
  auto d = dynamic_cast<ClassDefinition *>(_ast[ctx->class_definition()]);
  assert(d != nullptr);
  d->final(ctx->FINAL() != nullptr);
  _ast[ctx] = d;
}
void Compiler::enterClass_definition(ModelicaParser::Class_definitionContext *ctx) {

}
void Compiler::exitClass_definition(ModelicaParser::Class_definitionContext *ctx) {
  // use class definition in class_specifier, but append prefixes and encapsulated
  // TODO encapsulated not in AST
  //bool encapsulated = ctx->ENCAPSULATED() != nullptr;
  std::string class_type = ctx->class_prefixes()->class_type()->getText();
  auto d = dynamic_cast<ClassDefinition *>(_ast[ctx->class_specifier()]);
  assert(d != nullptr);
  auto c = ClassContents(ctx->class_prefixes()->class_type()->getText());
  c.partial(ctx->class_prefixes()->PARTIAL() != nullptr);
  d->class_(c);
  _ast[ctx] = d;
}
void Compiler::enterClass_prefixes(ModelicaParser::Class_prefixesContext *ctx) {

}
void Compiler::exitClass_prefixes(ModelicaParser::Class_prefixesContext *ctx) {

}
void Compiler::enterClass_type(ModelicaParser::Class_typeContext *ctx) {

}
void Compiler::exitClass_type(ModelicaParser::Class_typeContext *ctx) {

}
void Compiler::enterClass_spec_comp(ModelicaParser::Class_spec_compContext *ctx) {

}
void Compiler::exitClass_spec_comp(ModelicaParser::Class_spec_compContext *ctx) {
  std::string name = ctx->IDENT()[0]->getText();
  std::string name_end = ctx->IDENT()[1]->getText();
  if (name != name_end) throw std::runtime_error("class identifiers do not match");
  auto c = new ClassDefinition(name);
  c->comment(ctx->string_comment()->getText());
  _ast[ctx] = c;
}
void Compiler::enterClass_spec_base(ModelicaParser::Class_spec_baseContext *ctx) {

}
void Compiler::exitClass_spec_base(ModelicaParser::Class_spec_baseContext *ctx) {

}
void Compiler::enterClass_spec_enum(ModelicaParser::Class_spec_enumContext *ctx) {

}
void Compiler::exitClass_spec_enum(ModelicaParser::Class_spec_enumContext *ctx) {

}
void Compiler::enterClass_spec_der(ModelicaParser::Class_spec_derContext *ctx) {

}
void Compiler::exitClass_spec_der(ModelicaParser::Class_spec_derContext *ctx) {

}
void Compiler::enterClass_spec_extends(ModelicaParser::Class_spec_extendsContext *ctx) {

}
void Compiler::exitClass_spec_extends(ModelicaParser::Class_spec_extendsContext *ctx) {

}
void Compiler::enterBase_prefix(ModelicaParser::Base_prefixContext *ctx) {

}
void Compiler::exitBase_prefix(ModelicaParser::Base_prefixContext *ctx) {

}
void Compiler::enterEnum_list(ModelicaParser::Enum_listContext *ctx) {

}
void Compiler::exitEnum_list(ModelicaParser::Enum_listContext *ctx) {

}
void Compiler::enterEnumeration_literal(ModelicaParser::Enumeration_literalContext *ctx) {

}
void Compiler::exitEnumeration_literal(ModelicaParser::Enumeration_literalContext *ctx) {

}
void Compiler::enterComposition(ModelicaParser::CompositionContext *ctx) {

}
void Compiler::exitComposition(ModelicaParser::CompositionContext *ctx) {

}
void Compiler::enterLanguage_specification(ModelicaParser::Language_specificationContext *ctx) {

}
void Compiler::exitLanguage_specification(ModelicaParser::Language_specificationContext *ctx) {

}
void Compiler::enterExternal_function_call(ModelicaParser::External_function_callContext *ctx) {

}
void Compiler::exitExternal_function_call(ModelicaParser::External_function_callContext *ctx) {

}
void Compiler::enterElement_list(ModelicaParser::Element_listContext *ctx) {

}
void Compiler::exitElement_list(ModelicaParser::Element_listContext *ctx) {

}
void Compiler::enterElement(ModelicaParser::ElementContext *ctx) {

}
void Compiler::exitElement(ModelicaParser::ElementContext *ctx) {

}
void Compiler::enterRegular_element(ModelicaParser::Regular_elementContext *ctx) {

}
void Compiler::exitRegular_element(ModelicaParser::Regular_elementContext *ctx) {

}
void Compiler::enterReplaceable_element(ModelicaParser::Replaceable_elementContext *ctx) {

}
void Compiler::exitReplaceable_element(ModelicaParser::Replaceable_elementContext *ctx) {

}
void Compiler::enterImport_clause(ModelicaParser::Import_clauseContext *ctx) {

}
void Compiler::exitImport_clause(ModelicaParser::Import_clauseContext *ctx) {

}
void Compiler::enterImport_list(ModelicaParser::Import_listContext *ctx) {

}
void Compiler::exitImport_list(ModelicaParser::Import_listContext *ctx) {

}
void Compiler::enterExtends_clause(ModelicaParser::Extends_clauseContext *ctx) {

}
void Compiler::exitExtends_clause(ModelicaParser::Extends_clauseContext *ctx) {

}
void Compiler::enterConstraining_clause(ModelicaParser::Constraining_clauseContext *ctx) {

}
void Compiler::exitConstraining_clause(ModelicaParser::Constraining_clauseContext *ctx) {

}
void Compiler::enterComponent_clause(ModelicaParser::Component_clauseContext *ctx) {

}
void Compiler::exitComponent_clause(ModelicaParser::Component_clauseContext *ctx) {

}
void Compiler::enterType_prefix(ModelicaParser::Type_prefixContext *ctx) {

}
void Compiler::exitType_prefix(ModelicaParser::Type_prefixContext *ctx) {

}
void Compiler::enterType_specifier_element(ModelicaParser::Type_specifier_elementContext *ctx) {

}
void Compiler::exitType_specifier_element(ModelicaParser::Type_specifier_elementContext *ctx) {

}
void Compiler::enterType_specifier(ModelicaParser::Type_specifierContext *ctx) {

}
void Compiler::exitType_specifier(ModelicaParser::Type_specifierContext *ctx) {

}
void Compiler::enterComponent_list(ModelicaParser::Component_listContext *ctx) {

}
void Compiler::exitComponent_list(ModelicaParser::Component_listContext *ctx) {

}
void Compiler::enterComponent_declaration(ModelicaParser::Component_declarationContext *ctx) {

}
void Compiler::exitComponent_declaration(ModelicaParser::Component_declarationContext *ctx) {

}
void Compiler::enterCondition_attribute(ModelicaParser::Condition_attributeContext *ctx) {

}
void Compiler::exitCondition_attribute(ModelicaParser::Condition_attributeContext *ctx) {

}
void Compiler::enterDeclaration(ModelicaParser::DeclarationContext *ctx) {

}
void Compiler::exitDeclaration(ModelicaParser::DeclarationContext *ctx) {

}
void Compiler::enterModification_class(ModelicaParser::Modification_classContext *ctx) {

}
void Compiler::exitModification_class(ModelicaParser::Modification_classContext *ctx) {

}
void Compiler::enterModification_assignment(ModelicaParser::Modification_assignmentContext *ctx) {

}
void Compiler::exitModification_assignment(ModelicaParser::Modification_assignmentContext *ctx) {

}
void Compiler::enterModification_assignment2(ModelicaParser::Modification_assignment2Context *ctx) {

}
void Compiler::exitModification_assignment2(ModelicaParser::Modification_assignment2Context *ctx) {

}
void Compiler::enterClass_modification(ModelicaParser::Class_modificationContext *ctx) {

}
void Compiler::exitClass_modification(ModelicaParser::Class_modificationContext *ctx) {

}
void Compiler::enterArgument_list(ModelicaParser::Argument_listContext *ctx) {

}
void Compiler::exitArgument_list(ModelicaParser::Argument_listContext *ctx) {

}
void Compiler::enterArgument(ModelicaParser::ArgumentContext *ctx) {

}
void Compiler::exitArgument(ModelicaParser::ArgumentContext *ctx) {

}
void Compiler::enterElement_modification_or_replaceable(ModelicaParser::Element_modification_or_replaceableContext *ctx) {

}
void Compiler::exitElement_modification_or_replaceable(ModelicaParser::Element_modification_or_replaceableContext *ctx) {

}
void Compiler::enterElement_modification(ModelicaParser::Element_modificationContext *ctx) {

}
void Compiler::exitElement_modification(ModelicaParser::Element_modificationContext *ctx) {

}
void Compiler::enterElement_redeclaration(ModelicaParser::Element_redeclarationContext *ctx) {

}
void Compiler::exitElement_redeclaration(ModelicaParser::Element_redeclarationContext *ctx) {

}
void Compiler::enterElement_replaceable(ModelicaParser::Element_replaceableContext *ctx) {

}
void Compiler::exitElement_replaceable(ModelicaParser::Element_replaceableContext *ctx) {

}
void Compiler::enterComponent_clause1(ModelicaParser::Component_clause1Context *ctx) {

}
void Compiler::exitComponent_clause1(ModelicaParser::Component_clause1Context *ctx) {

}
void Compiler::enterComponent_declaration1(ModelicaParser::Component_declaration1Context *ctx) {

}
void Compiler::exitComponent_declaration1(ModelicaParser::Component_declaration1Context *ctx) {

}
void Compiler::enterShort_class_definition(ModelicaParser::Short_class_definitionContext *ctx) {

}
void Compiler::exitShort_class_definition(ModelicaParser::Short_class_definitionContext *ctx) {

}
void Compiler::enterEquation_block(ModelicaParser::Equation_blockContext *ctx) {

}
void Compiler::exitEquation_block(ModelicaParser::Equation_blockContext *ctx) {

}
void Compiler::enterEquation_section(ModelicaParser::Equation_sectionContext *ctx) {

}
void Compiler::exitEquation_section(ModelicaParser::Equation_sectionContext *ctx) {

}
void Compiler::enterStatement_block(ModelicaParser::Statement_blockContext *ctx) {

}
void Compiler::exitStatement_block(ModelicaParser::Statement_blockContext *ctx) {

}
void Compiler::enterAlgorithm_section(ModelicaParser::Algorithm_sectionContext *ctx) {

}
void Compiler::exitAlgorithm_section(ModelicaParser::Algorithm_sectionContext *ctx) {

}
void Compiler::enterEquation_simple(ModelicaParser::Equation_simpleContext *ctx) {

}
void Compiler::exitEquation_simple(ModelicaParser::Equation_simpleContext *ctx) {

}
void Compiler::enterEquation_if(ModelicaParser::Equation_ifContext *ctx) {

}
void Compiler::exitEquation_if(ModelicaParser::Equation_ifContext *ctx) {

}
void Compiler::enterEquation_for(ModelicaParser::Equation_forContext *ctx) {

}
void Compiler::exitEquation_for(ModelicaParser::Equation_forContext *ctx) {

}
void Compiler::enterEquation_connect_clause(ModelicaParser::Equation_connect_clauseContext *ctx) {

}
void Compiler::exitEquation_connect_clause(ModelicaParser::Equation_connect_clauseContext *ctx) {

}
void Compiler::enterEquation_when(ModelicaParser::Equation_whenContext *ctx) {

}
void Compiler::exitEquation_when(ModelicaParser::Equation_whenContext *ctx) {

}
void Compiler::enterEquation_function(ModelicaParser::Equation_functionContext *ctx) {

}
void Compiler::exitEquation_function(ModelicaParser::Equation_functionContext *ctx) {

}
void Compiler::enterEquation(ModelicaParser::EquationContext *ctx) {

}
void Compiler::exitEquation(ModelicaParser::EquationContext *ctx) {

}
void Compiler::enterStatement_component_reference(ModelicaParser::Statement_component_referenceContext *ctx) {

}
void Compiler::exitStatement_component_reference(ModelicaParser::Statement_component_referenceContext *ctx) {

}
void Compiler::enterStatement_component_function(ModelicaParser::Statement_component_functionContext *ctx) {

}
void Compiler::exitStatement_component_function(ModelicaParser::Statement_component_functionContext *ctx) {

}
void Compiler::enterStatement_break(ModelicaParser::Statement_breakContext *ctx) {

}
void Compiler::exitStatement_break(ModelicaParser::Statement_breakContext *ctx) {

}
void Compiler::enterStatement_return(ModelicaParser::Statement_returnContext *ctx) {

}
void Compiler::exitStatement_return(ModelicaParser::Statement_returnContext *ctx) {

}
void Compiler::enterStatement_if(ModelicaParser::Statement_ifContext *ctx) {

}
void Compiler::exitStatement_if(ModelicaParser::Statement_ifContext *ctx) {

}
void Compiler::enterStatement_for(ModelicaParser::Statement_forContext *ctx) {

}
void Compiler::exitStatement_for(ModelicaParser::Statement_forContext *ctx) {

}
void Compiler::enterStatement_while(ModelicaParser::Statement_whileContext *ctx) {

}
void Compiler::exitStatement_while(ModelicaParser::Statement_whileContext *ctx) {

}
void Compiler::enterStatement_when(ModelicaParser::Statement_whenContext *ctx) {

}
void Compiler::exitStatement_when(ModelicaParser::Statement_whenContext *ctx) {

}
void Compiler::enterStatement(ModelicaParser::StatementContext *ctx) {

}
void Compiler::exitStatement(ModelicaParser::StatementContext *ctx) {

}
void Compiler::enterIf_equation(ModelicaParser::If_equationContext *ctx) {

}
void Compiler::exitIf_equation(ModelicaParser::If_equationContext *ctx) {

}
void Compiler::enterIf_statement(ModelicaParser::If_statementContext *ctx) {

}
void Compiler::exitIf_statement(ModelicaParser::If_statementContext *ctx) {

}
void Compiler::enterFor_equation(ModelicaParser::For_equationContext *ctx) {

}
void Compiler::exitFor_equation(ModelicaParser::For_equationContext *ctx) {

}
void Compiler::enterFor_statement(ModelicaParser::For_statementContext *ctx) {

}
void Compiler::exitFor_statement(ModelicaParser::For_statementContext *ctx) {

}
void Compiler::enterFor_indices(ModelicaParser::For_indicesContext *ctx) {

}
void Compiler::exitFor_indices(ModelicaParser::For_indicesContext *ctx) {

}
void Compiler::enterFor_index(ModelicaParser::For_indexContext *ctx) {

}
void Compiler::exitFor_index(ModelicaParser::For_indexContext *ctx) {

}
void Compiler::enterWhile_statement(ModelicaParser::While_statementContext *ctx) {

}
void Compiler::exitWhile_statement(ModelicaParser::While_statementContext *ctx) {

}
void Compiler::enterWhen_equation(ModelicaParser::When_equationContext *ctx) {

}
void Compiler::exitWhen_equation(ModelicaParser::When_equationContext *ctx) {

}
void Compiler::enterWhen_statement(ModelicaParser::When_statementContext *ctx) {

}
void Compiler::exitWhen_statement(ModelicaParser::When_statementContext *ctx) {

}
void Compiler::enterConnect_clause(ModelicaParser::Connect_clauseContext *ctx) {

}
void Compiler::exitConnect_clause(ModelicaParser::Connect_clauseContext *ctx) {

}
void Compiler::enterExpression_simple(ModelicaParser::Expression_simpleContext *ctx) {

}
void Compiler::exitExpression_simple(ModelicaParser::Expression_simpleContext *ctx) {

}
void Compiler::enterExpression_if(ModelicaParser::Expression_ifContext *ctx) {

}
void Compiler::exitExpression_if(ModelicaParser::Expression_ifContext *ctx) {

}
void Compiler::enterSimple_expression(ModelicaParser::Simple_expressionContext *ctx) {

}
void Compiler::exitSimple_expression(ModelicaParser::Simple_expressionContext *ctx) {

}
void Compiler::enterExpr_neg(ModelicaParser::Expr_negContext *ctx) {

}
void Compiler::exitExpr_neg(ModelicaParser::Expr_negContext *ctx) {

}
void Compiler::enterExpr_add(ModelicaParser::Expr_addContext *ctx) {

}
void Compiler::exitExpr_add(ModelicaParser::Expr_addContext *ctx) {

}
void Compiler::enterExpr_exp(ModelicaParser::Expr_expContext *ctx) {

}
void Compiler::exitExpr_exp(ModelicaParser::Expr_expContext *ctx) {

}
void Compiler::enterExpr_or(ModelicaParser::Expr_orContext *ctx) {

}
void Compiler::exitExpr_or(ModelicaParser::Expr_orContext *ctx) {

}
void Compiler::enterExpr_primary(ModelicaParser::Expr_primaryContext *ctx) {

}
void Compiler::exitExpr_primary(ModelicaParser::Expr_primaryContext *ctx) {

}
void Compiler::enterExpr_and(ModelicaParser::Expr_andContext *ctx) {

}
void Compiler::exitExpr_and(ModelicaParser::Expr_andContext *ctx) {

}
void Compiler::enterExpr_rel(ModelicaParser::Expr_relContext *ctx) {

}
void Compiler::exitExpr_rel(ModelicaParser::Expr_relContext *ctx) {

}
void Compiler::enterExpr_not(ModelicaParser::Expr_notContext *ctx) {

}
void Compiler::exitExpr_not(ModelicaParser::Expr_notContext *ctx) {

}
void Compiler::enterExpr_mul(ModelicaParser::Expr_mulContext *ctx) {

}
void Compiler::exitExpr_mul(ModelicaParser::Expr_mulContext *ctx) {

}
void Compiler::enterPrimary_unsigned_number(ModelicaParser::Primary_unsigned_numberContext *ctx) {

}
void Compiler::exitPrimary_unsigned_number(ModelicaParser::Primary_unsigned_numberContext *ctx) {

}
void Compiler::enterPrimary_string(ModelicaParser::Primary_stringContext *ctx) {

}
void Compiler::exitPrimary_string(ModelicaParser::Primary_stringContext *ctx) {

}
void Compiler::enterPrimary_false(ModelicaParser::Primary_falseContext *ctx) {

}
void Compiler::exitPrimary_false(ModelicaParser::Primary_falseContext *ctx) {

}
void Compiler::enterPrimary_true(ModelicaParser::Primary_trueContext *ctx) {

}
void Compiler::exitPrimary_true(ModelicaParser::Primary_trueContext *ctx) {

}
void Compiler::enterPrimary_function(ModelicaParser::Primary_functionContext *ctx) {

}
void Compiler::exitPrimary_function(ModelicaParser::Primary_functionContext *ctx) {

}
void Compiler::enterPrimary_derivative(ModelicaParser::Primary_derivativeContext *ctx) {

}
void Compiler::exitPrimary_derivative(ModelicaParser::Primary_derivativeContext *ctx) {

}
void Compiler::enterPrimary_initial(ModelicaParser::Primary_initialContext *ctx) {

}
void Compiler::exitPrimary_initial(ModelicaParser::Primary_initialContext *ctx) {

}
void Compiler::enterPrimary_component_reference(ModelicaParser::Primary_component_referenceContext *ctx) {

}
void Compiler::exitPrimary_component_reference(ModelicaParser::Primary_component_referenceContext *ctx) {

}
void Compiler::enterPrimary_output_expression_list(ModelicaParser::Primary_output_expression_listContext *ctx) {

}
void Compiler::exitPrimary_output_expression_list(ModelicaParser::Primary_output_expression_listContext *ctx) {

}
void Compiler::enterPrimary_expression_list(ModelicaParser::Primary_expression_listContext *ctx) {

}
void Compiler::exitPrimary_expression_list(ModelicaParser::Primary_expression_listContext *ctx) {

}
void Compiler::enterPrimary_function_arguments(ModelicaParser::Primary_function_argumentsContext *ctx) {

}
void Compiler::exitPrimary_function_arguments(ModelicaParser::Primary_function_argumentsContext *ctx) {

}
void Compiler::enterPrimary_end(ModelicaParser::Primary_endContext *ctx) {

}
void Compiler::exitPrimary_end(ModelicaParser::Primary_endContext *ctx) {

}
void Compiler::enterName(ModelicaParser::NameContext *ctx) {

}
void Compiler::exitName(ModelicaParser::NameContext *ctx) {

}
void Compiler::enterComponent_reference_element(ModelicaParser::Component_reference_elementContext *ctx) {

}
void Compiler::exitComponent_reference_element(ModelicaParser::Component_reference_elementContext *ctx) {

}
void Compiler::enterComponent_reference(ModelicaParser::Component_referenceContext *ctx) {

}
void Compiler::exitComponent_reference(ModelicaParser::Component_referenceContext *ctx) {

}
void Compiler::enterFunction_call_args(ModelicaParser::Function_call_argsContext *ctx) {

}
void Compiler::exitFunction_call_args(ModelicaParser::Function_call_argsContext *ctx) {

}
void Compiler::enterFunction_arguments(ModelicaParser::Function_argumentsContext *ctx) {

}
void Compiler::exitFunction_arguments(ModelicaParser::Function_argumentsContext *ctx) {

}
void Compiler::enterNamed_arguments(ModelicaParser::Named_argumentsContext *ctx) {

}
void Compiler::exitNamed_arguments(ModelicaParser::Named_argumentsContext *ctx) {

}
void Compiler::enterNamed_argument(ModelicaParser::Named_argumentContext *ctx) {

}
void Compiler::exitNamed_argument(ModelicaParser::Named_argumentContext *ctx) {

}
void Compiler::enterArgument_function(ModelicaParser::Argument_functionContext *ctx) {

}
void Compiler::exitArgument_function(ModelicaParser::Argument_functionContext *ctx) {

}
void Compiler::enterArgument_expression(ModelicaParser::Argument_expressionContext *ctx) {

}
void Compiler::exitArgument_expression(ModelicaParser::Argument_expressionContext *ctx) {

}
void Compiler::enterOutput_expression_list(ModelicaParser::Output_expression_listContext *ctx) {

}
void Compiler::exitOutput_expression_list(ModelicaParser::Output_expression_listContext *ctx) {

}
void Compiler::enterExpression_list(ModelicaParser::Expression_listContext *ctx) {

}
void Compiler::exitExpression_list(ModelicaParser::Expression_listContext *ctx) {

}
void Compiler::enterArray_subscripts(ModelicaParser::Array_subscriptsContext *ctx) {

}
void Compiler::exitArray_subscripts(ModelicaParser::Array_subscriptsContext *ctx) {

}
void Compiler::enterSubscript(ModelicaParser::SubscriptContext *ctx) {

}
void Compiler::exitSubscript(ModelicaParser::SubscriptContext *ctx) {

}
void Compiler::enterComment(ModelicaParser::CommentContext *ctx) {

}
void Compiler::exitComment(ModelicaParser::CommentContext *ctx) {

}
void Compiler::enterString_comment(ModelicaParser::String_commentContext *ctx) {

}
void Compiler::exitString_comment(ModelicaParser::String_commentContext *ctx) {

}
void Compiler::enterAnnotation(ModelicaParser::AnnotationContext *ctx) {

}
void Compiler::exitAnnotation(ModelicaParser::AnnotationContext *ctx) {

}

} // cymoca