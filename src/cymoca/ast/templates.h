#ifndef CYMOCA_AST_TEMPLATES_H_
#define CYMOCA_AST_TEMPLATES_H_

#include "node.h"

#include <memory>

namespace cymoca::ast {

template <class Choice>
struct Option : Node {
  virtual std::unique_ptr<Option> clone() = 0;
  Choice *choice{};
};

/**
 * The Choice template provides a method for generic types within the
 * Abstract Syntax Tree. It employs unique_ptrs to store the concrete
 * node implementation which derives from the Option parameter. A
 * virtual clone method is used to allow users to clone the underlying
 * concrete class. All visitation methods are also forwarded to the
 * concrete class.
 * @tparam Label a non-used struct just to differentiate the templates
 * for type checking, this is how statements in the AST are classified
 */
template <class Label>
class Choice : public Node {
 public:
  Choice(std::unique_ptr<Option<Choice>> option) : m_option(std::move(option)) {
    option->set_choice(this);
  }
  ~Choice() = default;
  template <typename T>
  Choice(T val) : m_option{std::make_unique<T>(std::move(val))} {
    m_option->choice = this;
  }
  Choice(const Choice &other) : m_option{other.m_option->clone()} {
    m_option->choice = this;
  }
  Choice(Choice &&other) : m_option{other.m_option.release()} {
    m_option->choice = this;
  };
  Choice &operator=(const Choice &other) {
    m_option.release();
    m_option = other.option->clone();
    m_option->choice = this;
    return *this;
  };
  Choice &operator=(Choice &&other) {
    m_option.reset(other.m_option.release());
    m_option->choice = this;
    return *this;
  };
  // pass through node functionality to option
  void enter(Listener &l) override { m_option->enter(l); }
  void exit(Listener &l) override { m_option->exit(l); }
  std::vector<Node *> children() override { return m_option->children(); }
  template <typename T>
  void set_option(T val) {
    m_option = std::make_unique<T>(std::move(val));
    m_option->choice = this;
  }
  Option<Choice> &option() {
    assert(m_option);
    return *m_option;
  }

 private:
  std::unique_ptr<Option<Choice>> m_option;
};

/**
 * A binary template.
 */
template <class Left, class Right, class Base, class Label>
struct Binary : Base {
  NODE
  /**
   * @brief Binary constructor
   * @param left the left expression
   * @param right the right expression
   */
  Binary(Left left, Right right)
      : left(std::move(left)), right(std::move(right)) {}

  std::unique_ptr<Base> clone() override {
    return std::make_unique<Binary>(left, right);
  }
  std::vector<Node *> children() override { return {&left, &right}; }
  Left left;
  Right right;
};

/**
 * A unary template.
 */
template <class Right, class Base, class Label>
struct Unary : Base {
  NODE
  /**
   * @brief Unary constructor
   * @param right the right expression
   */
  Unary(Right right)
      : right(std::move(right)) {}

  std::unique_ptr<Base> clone() override {
    return std::make_unique<Unary>(right);
  }
  std::vector<Node *> children() override { return {&right}; }
  Right right;
};

/**
 * A value template.
 */
template <class Type, class Base>
struct Value : Base {
  NODE Value(Type value) : value(value) {}
  std::unique_ptr<Base> clone() override {
    return std::make_unique<Value>(value);
  }
  std::vector<Node *> children() override { return {}; }
  Type value;
};

template <class Item>
struct ConditionBlock : Node {
  NODE ConditionBlock(Condition condition, std::vector<Item> items)
      : condition(std::move(condition)), items(std::move(items)) {}
  std::vector<Node *> children() override {
    std::vector<Node *> v;
    v.push_back(&condition);
    for (auto &item : items) {
      v.push_back(&item);
    }
    return v;
  }
  Condition condition;
  std::vector<Item> items;
};

template <class Element, class Base, class Label>
struct ConditionBlocks : Base {
  NODE using Block = ConditionBlock<Element>;
  /**
   * @brief This ctor exists to allow direct initialization
   * of the vector and skip a level of braces.
   * @param value the intializer list
   */
  ConditionBlocks(std::initializer_list<Block> blocks)
      : blocks(std::move(blocks)) {}
  /**
   * @brief vector initializer.
   * @param blocks the vector passed in.
   */
  ConditionBlocks(std::vector<Block> blocks) : blocks(std::move(blocks)) {}
  std::unique_ptr<Base> clone() override {
    return std::make_unique<ConditionBlocks>(blocks);
  }
  std::vector<Node *> children() override {
    std::vector<Node *> v;
    for (auto &b : blocks) {
      // add all conditions
      v.push_back(&b);
    }
    return v;
  }
  std::vector<Block> blocks;
};

}  // namespace cymoca::ast

#endif
