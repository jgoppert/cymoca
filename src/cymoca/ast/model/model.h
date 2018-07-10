#ifndef CYMOCA_AST_MODEL_MODEL_H_
#define CYMOCA_AST_MODEL_MODEL_H_

#include "../node.h"

#include <unordered_map>

namespace cymoca::ast::model {

/**
 * A dictionary of elements with names as keys and
 * allocated element instances as values.
 */
class ElementDict : public Base {
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
class Class : public Base {
 protected:
  std::unique_ptr<ElementDict> m_elements{};
  std::unique_ptr<equation::List> m_equations{};

 public:
  NODE_MACRO
  Class(std::unique_ptr<ElementDict> elements,
        std::unique_ptr<equation::List> equations)
      : m_elements(std::move(elements)), m_equations(std::move(equations)) {}
  std::vector<INode *> getChildren() override {
    return {m_elements.get(), m_equations.get()};
  }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Class>(m_elements->cloneAs<ElementDict>(),
                                   m_equations->cloneAs<equation::List>());
  }
};

}  // namespace cymoca::ast::model
#endif
