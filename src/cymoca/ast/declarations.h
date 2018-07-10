#ifndef CYMOCA_AST_DECLARATIONS_H_
#define CYMOCA_AST_DECLARATIONS_H_

namespace cymoca::ast {

class INode;

namespace model {
class ElementDict;
class Class;
}  // namespace model

namespace listener {
class Base;
}  // namespace listener

namespace expression {
class Add;
class Base;
class Divide;
class Multiply;
class Number;
class Reference;
class Subtract;
}  // namespace expression

namespace condition {
class And;
class Base;
class Boolean;
class Equal;
class GreaterThan;
class GreaterThanOrEqual;
class LessThan;
class LessThanOrEqual;
class Not;
class Or;
}  // namespace condition

namespace equation {
class Base;
class Block;
class If;
class List;
class Simple;
class When;
}  // namespace equation

namespace statement {
class Base;
class Block;
class If;
class When;
}  // namespace statement

namespace element {
class Component;
}  // namespace element

}  // namespace cymoca::ast

#endif
