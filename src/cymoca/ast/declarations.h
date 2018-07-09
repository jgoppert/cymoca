#ifndef CYMOCA_AST_DECLARATIONS_H_
#define CYMOCA_AST_DECLARATIONS_H_

namespace cymoca::ast {

class INode;
class ElementDict;
class Class;

namespace listener {
class Base;
}  // namespace listener

namespace expression {
class Base;
class Add;
class Subtract;
class Multiply;
class Divide;
class Reference;
class Number;
}  // namespace expression

namespace condition {
class Base;
class Boolean;
class And;
class Or;
class Not;
class LessThan;
class LessThanOrEqual;
class GreaterThan;
class GreaterThanOrEqual;
class Equal;
}  // namespace condition

namespace equation {
class Base;
class Block;
class Simple;
class If;
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
