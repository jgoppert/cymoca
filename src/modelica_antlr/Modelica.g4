grammar Modelica;

// Modelica 3.4

// B.2.1 Stored Definition - Within
stored_definition:
    (WITHIN name? ';')?
    (FINAL? class_definition ';')*;

// B.2.2 Class Definition
class_definition:
    ENCAPSULATED? class_prefixes class_specifier;

class_prefixes:
    PARTIAL?
    (CLASS | MODEL | OPERATOR? RECORD | BLOCK | EXPANDABLE |
        CONNECTOR | TYPE | PACKAGE |
        (PURE | IMPURE)? OPERATOR? FUNCTION | OPERATOR);

class_specifier:
    long_class_specifier | short_class_specifier | der_class_specifier;

long_class_specifier:
    IDENT string_comment? composition END IDENT
    | EXTENDS IDENT class_modification? string_comment? composition END IDENT;

short_class_specifier:
    IDENT '=' base_prefix type_specifier array_subscripts?
        class_modification? comment?
    | IDENT '=' ENUMERATION '(' (enum_list?  | ':') ')' comment?;

der_class_specifier:
    IDENT '=' DER '(' type_specifier ',' IDENT ( ',' IDENT )* ')' comment?;

base_prefix:
    (INPUT | OUTPUT)?;

enum_list:
    enumeration_literal (',' enumeration_literal)*;

enumeration_literal:
    IDENT comment?;

composition:
    (private_elem+=element ';')*
    (PUBLIC (public_elem+=element ';')*
        | PROTECTED (protected_elem+=element ';')*
        | equation_section
        | algorithm_section
    )*
    (EXTERNAL language_specification? external_function_call? annotation? ';')?
    (annotation ';')?;

language_specification:
    STRING;

external_function_call:
    (component_reference '=')? IDENT '(' expression_list? ')';

element:
    IMPORT (IDENT '=' name | name ('.' ('*' |
        '{' import_list '}' ) ) ? ) comment?                   # elem_import
    | EXTENDS type_specifier class_modification? annotation?   # elem_extends
    | REDECLARE? FINAL? INNER? OUTER? (
        class_definition
        | REPLACEABLE? class_definition
        (constraining_clause comment?)?)                       # elem_class
    | REDECLARE? FINAL? INNER? OUTER? (
        component_clause
        | REPLACEABLE? component_clause
        (constraining_clause comment?)?)                       # elem_comp
    ;

import_list:
    IDENT (',' IDENT);

// B.2.3 Extends

// moved expanded extends_clause to element to compress parse tree

constraining_clause:
    CONSTRAINEDBY type_specifier class_modification?;

// B.2.4 Component Clause
component_clause:
    type_prefix type_specifier array_subscripts? component_list;

type_prefix:
    (FLOW | STREAM)? (DISCRETE | PARAMETER | CONSTANT)? (INPUT | OUTPUT)?;

component_list:
    component_declaration (',' component_declaration)*;

component_declaration:
    IDENT array_subscripts? modification? condition_attribute? comment?;

condition_attribute:
    IF condition;

// B.2.5 Modification
modification:
    class_modification ('=' expression)?
    | '=' expression
    | ':=' expression;

class_modification:
    '(' argument_list? ')';

argument_list:
    argument (',' argument)*;

argument:
    element_modification_or_replaceable | element_modification;

element_modification_or_replaceable:
    EACH? FINAL? (element_modification | element_replaceable);

element_modification:
    name modification? string_comment?;

// Not used, but in Spec?
//element_redeclaration:
//    REDECLARE EACH? FINAL? (
//        short_class_definition
//        | component_clause1
//        | element_replaceable);

element_replaceable:
    REPLACEABLE (short_class_definition | component_clause1)
        constraining_clause?;

// expand component_declaration1 and embed in component_clause1 to
// avoid extra level of parse tree
component_clause1:
    type_prefix type_specifier IDENT array_subscripts? modification? comment?;


short_class_definition:
    class_prefixes short_class_specifier;

// B.2.6 Equations

// added to group eq by block
eq_block:
    (equation ';')*;

// added to group eq by block
stmt_block:
    (statement ';')*;

// don't use eq_block here to avoid extra level of parse tree
equation_section:
    INITIAL? EQUATION (equation ';')*;

// don't use statement_block here to avoid extra level of parse tree
algorithm_section:
    INITIAL? ALGORITHM (statement ';')*;

equation:
    expr '=' expression comment?                           # eq_simple
    | IF '(' condition ')' THEN
        eq_block
      (ELSEIF '(' condition ')' THEN
        eq_block
      )*
      (ELSE
        eq_block
      )?
       END IF comment?                                      # eq_if
    | FOR for_indices LOOP
        (equation ';')*
      END FOR comment?                                      # eq_for
    | CONNECT '(' component_reference ','
                    component_reference ')' comment?        # eq_connect
    | WHEN '(' condition ')' THEN
        eq_block
      (ELSEWHEN '(' condition ')' THEN
        eq_block
      )*
      END WHEN comment?                                     # eq_when
    | component_reference function_call_args comment?       # eq_func
    ;

statement:
    component_reference (':=' expression |
            function_call_args) comment?                    # stmt_ref
    | '(' output_expression_list ')' ':='
        component_reference function_call_args comment?     # stmt_func
    | keyword=(BREAK|RETURN)  comment?                      # stmt_key
    | IF '(' condition ')' THEN
        stmt_block
      (ELSEIF '(' condition ')' THEN
        stmt_block
      )*
      (ELSE
        stmt_block
      )?
      END IF comment?                                       # stmt_if
    | FOR for_indices LOOP
        (statement ';')*
      END FOR comment?                                      # stmt_for
    | WHILE '(' condition ')' LOOP
        (statement ';')*
      END WHEN comment?                                     # stmt_while
    | WHEN '(' condition ')' THEN
        stmt_block
      (ELSEWHEN '(' condition ')' THEN
        stmt_block
      )*
      END WHEN comment?                                     # stmt_when
    ;

for_indices:
    for_index (',' for_index);

for_index:
    IDENT (IN expression)?;

// B.2.7 Expressions
expression:
    expr                                                    # expr_simple
    | IF condition THEN expr
        (ELSEIF expr THEN expr)* ELSE expr                  # expr_if
    ;

// this is a reformulation of the standard to remove extra levels
// of the parse tree that exist due to precedence and can be reformulated
// by ordering the recursive rules
expr :
    UNSIGNED_NUMBER                                         # expr_number
    | STRING                                                # expr_string
    //| END                                                   # expr_end
    | component_reference function_call_args                # expr_func
    | func=(DER | INITIAL | PURE) function_call_args        # expr_func
    | component_reference                                   # expr_ref
    //| '(' output_expression_list ')'                        # expr_output
    //| '[' expression_list (';' expression_list)* ']'        # expr_list
    //| '{' array_arguments '}'                               # expr_array
    | expr op=('^' | '.^') expr                             # expr_binary
    | expr op=('*' | '/' | '.*' | './') expr                # expr_binary
    | op='-' expr                                           # expr_unary
    | expr op=('+' | '-' | '.+' | '.-') expr                # expr_binary
    | expr ( ':' expr ( ':' expr )? )                       # expr_range
    ;

// this is a reformulation of the standard to remove extra levels
// of the parse tree that exist due to precedence and can be reformulated
// by ordering the recursive rules
condition :
    value=(TRUE | FALSE)                                    # cond_bool
    | component_reference function_call_args                # cond_func
    | component_reference                                   # cond_ref
    | expr op=('<' | '<=' | '>' | '>=' | '==' | '<>') expr  # cond_compare
    | op='not' condition                                    # cond_unary
    | condition op='and' condition                          # cond_binary
    | condition op='or' condition                           # cond_binary
    ;

// expand name gramamr in type_specifier to remove level of parse tree
type_specifier: '.'? IDENT ('.' IDENT)*;

name: IDENT ('.' IDENT)*;

component_reference:
    '.'? IDENT array_subscripts? ('.' IDENT array_subscripts?)*;

function_call_args:
    '(' function_arguments? ')';

function_arguments:
    expression (',' function_argumments_non_first |
            FOR for_indices)?                           # args_expr
    | FUNCTION name '(' named_arguments? ')'
            (',' function_argumments_non_first)?        # args_func
    | named_arguments                                   # args_named
    ;

function_argumments_non_first:
    function_argument (',' function_argumments_non_first)?
    | named_arguments;

array_arguments:
    expression (',' array_arguments_non_first | FOR for_indices)?;

array_arguments_non_first:
    expression (',' array_arguments_non_first)?;

// standard uses recursion here, not necessary
named_arguments: named_argument (',' named_argument)*;

named_argument: IDENT '=' function_argument;

function_argument:
    FUNCTION name '(' named_arguments? ')' | expression;

output_expression_list:
    expression? (',' expression?)*;

expression_list:
    expression (',' expression)*;

array_subscripts:
    '[' subscript (',' subscript)* ']';

subscript:
    ':' | expression;

// modify comment to require either a string comment, annotation, or both,
// not allowing empty to trim parse tree of empty comment nodes
comment:
    string_comment | annotation | (string_comment annotation);

string_comment:
    (STRING ('+' STRING)*)+;

annotation:
    ANNOTATION class_modification;

//=========================================================
// Keywords
//=========================================================
ALGORITHM: 'algorithm';
ANNOTATION: 'annotation';
AND: 'and';
BLOCK: 'block';
BREAK: 'break';
CONSTRAINEDBY: 'constrained by';
CLASS: 'class';
CONSTANT: 'constant';
CONNECT: 'connect';
CONNECTOR: 'connector';
DER: 'der';
DISCRETE: 'discrete';
EACH : 'each';
ELSE: 'else';
ELSEIF: 'else if';
ELSEWHEN: 'else when';
ENCAPSULATED : 'encapsulated';
END: 'end';
ENUMERATION: 'enumeration';
EQUATION : 'equation';
EXPANDABLE: 'expandable';
EXTENDS: 'extends';
EXTERNAL: 'external';
FALSE : 'false';
FINAL : 'final';
FLOW: 'flow';
FOR: 'for';
FUNCTION: 'function';
IF: 'if';
IN: 'in';
IMPORT: 'import';
IMPURE: 'impure';
INITIAL : 'initial';
INPUT: 'input';
INNER : 'inner';
LOOP: 'loop';
MODEL: 'model';
NOT: 'not';
OPERATOR: 'operator';
OUTPUT: 'output';
OUTER : 'outer';
OR: 'or';
PACKAGE: 'package';
PARAMETER: 'parameter';
PARTIAL : 'partial';
PROTECTED: 'protected';
PURE: 'pure';
PUBLIC: 'public';
RECORD: 'record';
REDECLARE : 'redeclare';
REPLACEABLE: 'replaceable';
RETURN: 'return';
STREAM: 'stream';
THEN: 'then';
TRUE : 'true';
TYPE: 'type';
WHEN : 'when';
WHILE : 'while';
WITHIN : 'within';

//=========================================================
// Strings/ Numbers
//=========================================================
IDENT : NONDIGIT ( DIGIT | NONDIGIT )* | Q_IDENT;
STRING : '"' ('\\"' | ~('"'))* '"';
//STRING : '"' (S_CHAR | S_ESCAPE | ' ')* '"';
UNSIGNED_NUMBER : UNSIGNED_INTEGER  ( '.' UNSIGNED_NUMBER? )*
    ( [eE] [+-]? UNSIGNED_INTEGER)?;
COMMENT :
    ('/' '/' .*? '\n' | '/*' .*? '*/') -> channel(HIDDEN)
    ;
WS  :   [ \r\n\t]+ -> skip ; // toss out whitespace

//=========================================================
// Fragments
//=========================================================
fragment Q_IDENT : '\'' ( Q_CHAR | S_ESCAPE)+;
fragment NONDIGIT : [_a-zA-Z];
fragment S_CHAR : [\u0000-\u00FF];
fragment Q_CHAR : NONDIGIT | DIGIT | [!#$%&()*+,-./:;<>=?@[\]^{}| ];
fragment S_ESCAPE : ('\\\'' | '\\"' | '\\\\' | '\\?' | '\\b' |
    '\\f' | '\\n' | '\\r' | '\\t' | '\\v' | '\\a');
fragment DIGIT :  [0-9];
fragment UNSIGNED_INTEGER : DIGIT+;
// vi:ts=4:sw=4:expandtab:
