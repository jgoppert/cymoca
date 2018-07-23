#ifndef CYMOCA_AST_CUSTOM_H_
#define CYMOCA_AST_CUSTOM_H_

/**
 * Any nodes that don't follow a standard template type
 * are considered custom and are implemented here.
 */

#include "node.h"
#include "templates.h"

namespace cymoca::ast {

namespace expression {

/**
 * @brief The Ref struct represents a component reference.
 */
struct Reference : Option<Expression> {
  NODE Reference(std::vector<std::string> value) : value(std::move(value)) {}
  /**
   * @brief Ref this ctor exists to allow direct initialization
   * of the vector and skip a level of {}
   * @param value the intializer list
   */
  Reference(std::initializer_list<std::string> value)
      : value(std::move(value)) {}
  std::unique_ptr<Option<Expression>> clone() override {
    return std::make_unique<Reference>(value);
  }
  std::vector<Node *> children() override { return {}; }
  std::vector<std::string> value;
};

/**
 * @brief The Function struct is an expression representing a function
 * reference and argument list.
 */
struct Function : Option<Expression> {
  NODE Function(Reference ref, std::vector<Expression> args)
      : ref(std::move(ref)), args(std::move(args)) {}
  std::unique_ptr<Option<Expression>> clone() override {
    return std::make_unique<Function>(ref, args);
  }
  std::vector<Node *> children() override {
    std::vector<Node *> v;
    v.push_back(&ref);
    for (auto &a : args) {
      v.push_back(&a);
    }
    return v;
  }
  Reference ref;
  std::vector<Expression> args;
};

}  // namespace expression

namespace condition {}  // namespace condition

namespace equation {}  // namespace equation

namespace model {
struct Class : Node {
  NODE Class(std::vector<Equation> eqs) : eqs(std::move(eqs)){};
  Class(std::initializer_list<Equation> eqs) : eqs(std::move(eqs)){};
  std::vector<Node *> children() override {
    std::vector<Node *> v;
    for (auto &eq : eqs) {
      v.push_back(&eq);
    }
    return v;
  }
  std::vector<Equation> eqs{};
};

}  // namespace model

}  // namespace cymoca::ast

#endif
