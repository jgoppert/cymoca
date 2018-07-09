#ifndef CYMOCA_AST_EQUATION_EQUATION_H_
#define CYMOCA_AST_EQUATION_EQUATION_H_

#include "../condition/condition.h"
#include "../expression/expression.h"
#include "../node.h"

namespace cymoca::ast::equation {

class Base : virtual public INode {};

class Block : public Base {
 protected:
  std::unique_ptr<condition::Base> condition{};
  std::vector<std::unique_ptr<Base>> equations{};

 public:
  NODE_MACRO
  template <class... Args>
  explicit Block(std::unique_ptr<condition::Base> condition, Args... args)
      : condition(std::move(condition)) {
    int dummy[1 + sizeof...(Args)] = {
        0, (equations.push_back(std::move(args)), 0)...};
    (void)dummy;
  }
  std::vector<INode *> getChildren() override {
    std::vector<INode *> v;
    v.push_back(condition.get());
    for (auto &e : equations) {
      v.push_back(e.get());
    }
    return v;
  }
  std::unique_ptr<INode> clone() const override {
    auto res = std::make_unique<Block>(condition->cloneAs<condition::Base>());
    for (auto &e : equations) {
      res->equations.push_back(e->cloneAs<Base>());
    }
    return std::move(res);
  }
};

class Simple : public Base {
 protected:
  std::unique_ptr<expression::Base> left;
  std::unique_ptr<expression::Base> right;

 public:
  NODE_MACRO
  explicit Simple(std::unique_ptr<expression::Base> left,
                  std::unique_ptr<expression::Base> right)
      : left(std::move(left)), right(std::move(right)) {}
  std::vector<INode *> getChildren() override {
    return {left.get(), right.get()};
  }
  std::unique_ptr<INode> clone() const override {
    return std::make_unique<Simple>(left->cloneAs<expression::Base>(),
                                    right->cloneAs<expression::Base>());
  }
};

class If : public List<Block, Base> {
 public:
  NODE_MACRO
  using List::List;
  std::unique_ptr<INode> clone() const override { return cloneList<If>(); }
};

class When : public Block {
 public:
  NODE_MACRO
  using Block::Block;
};

}  // namespace cymoca::ast::equation
#endif
