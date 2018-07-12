#ifndef CYMOCA_AST_EQUATION_EQUATION_H_
#define CYMOCA_AST_EQUATION_EQUATION_H_

#include "../node.h"

namespace cymoca::ast::equation {

/**
 * A list of equations
 */
class List : public TList<Base, Base> {
 public:
  NODE_MACRO
  using TList::TList;
  std::unique_ptr<INode> clone() const override { return cloneList<List>(); }
};

class Block : public Base {
 protected:
  std::unique_ptr<condition::Base> m_condition{};
  std::unique_ptr<List> m_equations{};

 public:
  NODE_MACRO
  Block(std::unique_ptr<condition::Base> condition,
        std::unique_ptr<List> equations)
      : m_condition(std::move(condition)), m_equations(std::move(equations)) {}
  std::vector<INode *> getChildren() override {
    return {m_condition.get(), m_equations.get()};
  }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Block>(m_condition->cloneAs<condition::Base>(),
                                   m_equations->cloneAs<List>());
  }
};

class Simple : public TBinary<expression::Base, expression::Base, Base> {
 public:
  NODE_MACRO
  using TBinary::TBinary;
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Simple>(m_left->cloneAs<expression::Base>(),
                                    m_right->cloneAs<expression::Base>());
  }
};

class If : public TList<Block, Base> {
 public:
  NODE_MACRO
  using TList::TList;
  std::unique_ptr<INode> clone() const override { return cloneList<If>(); }
};

class When : public TList<Block, Base> {
 public:
  NODE_MACRO
  using TList::TList;
  std::unique_ptr<INode> clone() const override { return cloneList<When>(); }
};

}  // namespace cymoca::ast::equation
#endif
