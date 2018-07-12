#ifndef CYMOCA_AST_MODEL_MODEL_H_
#define CYMOCA_AST_MODEL_MODEL_H_

#include "../node.h"
#include "../statement/statement.h"

#include <unordered_map>

namespace cymoca::ast::model {

/**
 * A dictionary of elements with names as keys and
 * allocated element instances as values.
 */
class ElementDict : public Base {
 protected:
  std::unordered_map<std::string, std::unique_ptr<element::Base>> m_map{};

 public:
  NODE_MACRO
  template <class... Args>
  explicit ElementDict(Args... args) {
    int dummy[1 + sizeof...(Args)] = {
        (m_map[args->getName()] = std::move(args), 0)...};
    (void)dummy;
  }

  void set(const std::string &name, std::unique_ptr<element::Base> element) {
    m_map[name] = std::move(element);
  }

  element::Base &get(const std::string &name) { return *m_map[name]; }

  std::vector<INode *> getChildren() override {
    std::vector<INode *> v;
    for (auto &c : m_map) {
      v.push_back(c.second.get());
    }
    return v;
  }
  std::unique_ptr<INode> clone() const override {
    auto res = std::make_unique<ElementDict>();
    for (auto &keval : m_map) {
      res->m_map[keval.first] = keval.second->cloneAs<element::Base>();
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
  std::unique_ptr<statement::List> m_statements{};

 public:
  NODE_MACRO
  Class(std::unique_ptr<ElementDict> elements,
        std::unique_ptr<equation::List> equations)
      : m_elements(std::move(elements)), m_equations(std::move(equations)) {}
  ElementDict &getElements() { return *m_elements; }
  equation::List &getEquations() { return *m_equations; }
  statement::List &getStatements() { return *m_statements; }
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
