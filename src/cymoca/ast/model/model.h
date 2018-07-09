#ifndef CYMOCA_AST_MODEL_MODEL_H_
#define CYMOCA_AST_MODEL_MODEL_H_

#include "../condition/condition.h"
#include "../element/element.h"
#include "../equation/equation.h"
#include "../expression/expression.h"
#include "../node.h"

#include <unordered_map>

namespace cymoca::ast {

/**
 * A dictionary of elements with names as keys and
 * allocated element instances as values.
 */
class ElementDict : virtual public INode {
 public:
  NODE_MACRO
  template <class... Args>
  explicit ElementDict(Args... args) {
    int dummy[1 + sizeof...(Args)] = {
        (map[args->getName()] = move(args), 0)...};
    (void)dummy;
  }
  std::unordered_map<std::string, std::unique_ptr<element::Base>> map{};
  std::vector<INode *> getChildren() override {
    std::vector<INode *> v;
    for (auto &c : map) {
      v.push_back(c.second.get());
    }
    return v;
  }
  std::unique_ptr<INode> clone() const override {
    auto res = std::make_unique<ElementDict>();
    for (auto &keval : map) {
      res->map[keval.first] = keval.second->cloneAs<element::Base>();
    }
    return res;
  }
};

/**
 * The top level class structure
 */
class Class : virtual public INode {
 public:
  NODE_MACRO
  std::unique_ptr<ElementDict> elements;
  std::vector<std::unique_ptr<equation::Base>> equations{};
  std::vector<INode *> getChildren() override { return {elements.get()}; }
};

}  // namespace cymoca::ast
#endif
