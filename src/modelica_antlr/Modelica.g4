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
    IDENT string_comment composition END IDENT
    | EXTENDS IDENT class_modification? string_comment composition END IDENT;

short_class_specifier:
    IDENT '=' base_prefix type_specifier array_subscripts? class_modification? comment
    | IDENT '=' ENUMERATION '(' (enum_list?  | ':') ')' comment;

der_class_specifier:
    IDENT '=' DER '(' type_specifier ',' IDENT ( ',' IDENT )* ')' comment;

base_prefix:
    (INPUT | OUTPUT)?;

enum_list:
    enumeration_literal (',' enumeration_literal)*;

enumeration_literal:
    IDENT comment;

composition:
    element_list
    (PUBLIC element_list
        | PROTECTED element_list
        | equation_section
        | algorithm_section
    )*
    (EXTERNAL language_specification? external_function_call? annotation? ';')?
    (annotation ';')?;

language_specification:
    STRING;

external_function_call:
    (component_reference '=')? IDENT '(' expression_list? ')';

element_list:
    (element ';')*;

element:
    import_clause           # element_import_clause
    | extends_clause        # element_extends_clause
    | REDECLARE? FINAL? INNER? OUTER? (
        class_definition
        | REPLACEABLE? class_definition
        (constraining_clause comment)?
    )  #element_class_definition
    | REDECLARE? FINAL? INNER? OUTER? (
        component_clause
        | REPLACEABLE? component_clause
        (constraining_clause comment)?
    )  #element_component_definition
    ;

import_clause:
    IMPORT (IDENT '=' name | name ('.' ('*' | '{' import_list '}' ) ) ? ) comment;

import_list:
    IDENT (',' IDENT);

// B.2.3 Extends
extends_clause:
    EXTENDS type_specifier class_modification? annotation?;

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
    declaration condition_attribute? comment;

condition_attribute:
    IF expression;

declaration:
    IDENT array_subscripts? modification?;

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
    name modification? string_comment;

element_redeclaration:
    REDECLARE EACH? FINAL? (
        short_class_definition
        | component_clause1
        | element_replaceable);

element_replaceable:
    REPLACEABLE (short_class_definition | component_clause1) constraining_clause?;

component_clause1:
    type_prefix type_specifier component_declaration1;

component_declaration1:
    declaration comment;

short_class_definition:
    class_prefixes short_class_specifier;

// B.2.6 Equations

// added to group equations by block
equation_list:
    (equation ';')*;

// added to group equations by block
statement_list:
    (statement ';')*;

equation_section:
    INITIAL? EQUATION equation_list;

algorithm_section:
    INITIAL? ALGORITHM statement_list;

equation_options:
    simple_expression '=' expression    # equation_simple
    | if_equation                       # equation_if
    | for_equation                      # equation_for
    | connect_clause                    # equation_connect
    | when_equation                     # equation_when
    | component_reference function_call_args  # equation_function
    ;

equation:
    equation_options comment;

statement_options:
    component_reference (':=' expression | function_call_args)      # statement_comp
    | '(' output_expression_list ')' ':=' component_reference function_call_args # statement_output
    | BREAK             # statement_break
    | RETURN            # statement_return
    | if_statement      # statement_if
    | for_statement     # statement_for
    | while_statement   # statement_while
    | when_statement    # statement_when
    ;

statement:
    statement_options comment;

if_equation:
    IF expression THEN
        if_blocks=equation_list
    (ELSEIF expression THEN
        elseif_blocks=equation_list
    )*
    (ELSE
        else_blocks=equation_list
    )?
    END IF;

if_statement:
    IF expression THEN
        if_blocks=statement_list
    (ELSEIF expression THEN
        elseif_blocks=statement_list
    )*
    (ELSE
        else_blocks=statement_list
    )?
    END IF;

for_equation:
    FOR for_indices LOOP
        equation_list
    END FOR;

for_statement:
    FOR for_indices LOOP
        statement_list
    END FOR;

for_indices:
    for_index (',' for_index);

for_index:
    IDENT (IN expression)?;

while_statement:
    WHEN expression LOOP
        (statement ';')*
    (ELSEWHEN expression THEN
        (statement ';')*
    )*
    END WHEN;

when_equation:
    WHEN expression THEN
        when_block=equation_list
    (ELSEWHEN expression THEN
        elsewhen_block=equation_list
    )*
    END WHEN;

when_statement:
    WHEN expression THEN
        when_block=statement_list
    (ELSEWHEN expression THEN
        elsewhen_block=statement_list
    )*
    END WHEN;

connect_clause:
    CONNECT '(' component_reference ',' component_reference ')';

// B.2.7 Expressions
expression:
    simple_expression  # expression_simple
    | IF expression THEN expression (ELSEIF expression THEN expression)* ELSE expression  # expression_if
    ;

simple_expression:
    expr ( ':' expr ( ':' expr )? )?;

// this is a reformulation of the standard to remove extra levels
// of the parse tree that exist due to precedence and can be reformulated
// by ordering the recursive rules
expr :
    UNSIGNED_NUMBER                                         # primary_unsigned_number
    | STRING                                                # primary_string
    | FALSE                                                 # primary_false
    | TRUE                                                  # primary_true
    | component_reference function_call_args                # primary_function
    | DER function_call_args                                # primary_der
    | INITIAL function_call_args                            # primary_initial
    | PURE function_call_args                               # primary_pure
    | component_reference                                   # primary_compoment_reference
    | '(' output_expression_list ')'                        # primary_output_expression_list
    | '[' expression_list (';' expression_list)* ']'        # primary_expression_list
    | '{' array_arguments '}'                               # primary_array
    | END                                                   # primary_end
    | expr op=('^' | '.^') expr                             # expr_factor
    | expr op=('*' | '/' | '.*' | './') expr                # expr_term
    | op='-' expr                                           # expr_negative
    | expr  op=('+' | '-' | '.+' | '.-') expr               # expr_arithmetic
    | expr  op=('<' | '<=' | '>' | '>=' | '==' | '<>') expr # expr_relation
    | 'not' expr                                            # expr_logical_factor
    | expr 'and' expr                                       # expr_logical_term
    | expr 'or' expr                                        # expr_logical
    ;

type_specifier: '.'? name;

name: IDENT ('.' IDENT)*;

component_reference:
    '.'? IDENT array_subscripts? ('.' IDENT array_subscripts?)*;

function_call_args:
    '(' function_arguments? ')';

function_arguments:
    expression (',' function_argumments_non_first | FOR for_indices)?
    | FUNCTION name '(' named_arguments? ')' (',' function_argumments_non_first)?
    | named_arguments;

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

comment:
    string_comment annotation?;

string_comment:
    (STRING ('+' STRING)*)?;

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
UNSIGNED_NUMBER : UNSIGNED_INTEGER  ( '.' UNSIGNED_NUMBER? )* ( [eE] [+-]? UNSIGNED_INTEGER)?;
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
