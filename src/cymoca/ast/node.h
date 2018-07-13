#ifndef CYMOCA_AST_NODE_H_
#define CYMOCA_AST_NODE_H_

#include <cassert>
#include <memory>
#include <vector>

#include "listener/listener.h"

/**
 * A macro for nodes to use to attach listener
 *
 * Nodes must also forward declare themselves in listener/listener.h
 */
#define NODE_MACRO                                                        \
  void enter(listener::Base &listener) override { listener.enter(this); } \
  void exit(listener::Base &listener) override { listener.exit(this); }

namespace cymoca::ast {

/**
 * The abstract sytnax tree node interface. All nodes
 * of the tree implment this interface. Base classes
 * should inherit virtually to allow nodes with multiple
 * types.
 */
class INode {
 public:
  virtual ~INode(){};
  /**
   * A double dispatch entrace hook for listeners.
   */
  virtual void enter(listener::Base &listener) = 0;
  /**
   * A double dispatch exit hook for listeners.
   */
  virtual void exit(listener::Base &listener) = 0;
  /**
   * Returns a list of children (non-owning).
   */
  virtual std::vector<INode *> getChildren() = 0;
  /**
   * Perform a deep copy of this node.
   */
  virtual std::unique_ptr<INode> clone() const = 0;
  /**
   * Perform a deep copy and cast to the template type.
   */
  template <class T>
  std::unique_ptr<T> cloneAs() {
    auto n = this->clone();
    auto p = std::unique_ptr<T>(dynamic_cast<T *>(n.get()));
    assert(p.get());
    n.release();
    return p;
  }
};

/**
 * A template for binary nodes.
 */
template <class Left, class Right, class Base>
class TBinary : public Base {
 protected:
  std::unique_ptr<Left> m_left{};
  std::unique_ptr<Right> m_right{};

 public:
  TBinary(std::unique_ptr<Left> left, std::unique_ptr<Right> right)
      : m_left(move(left)), m_right(move(right)) {}
  Left *getLeft() { return m_left.get(); }
  Right *getRight() { return m_right.get(); }
  std::vector<INode *> getChildren() override {
    return {m_left.get(), m_right.get()};
  }
  template <class T>
  std::unique_ptr<INode> cloneBinary() const {
    return std::make_unique<T>(
        dynamic_cast<INode *>(m_left.get())->cloneAs<Left>(),
        dynamic_cast<INode *>(m_right.get())->cloneAs<Right>());
  }
};

/**
 * A template for unary nodes.
 */
template <class Right, class Base>
class TUnary : public Base {
 protected:
  std::unique_ptr<Right> m_right{};

 public:
  explicit TUnary(std::unique_ptr<Right> right) : m_right(move(right)) {}
  Right *getRight() { return m_right.get(); }
  std::vector<INode *> getChildren() override { return {m_right.get()}; }
  template <class T>
  std::unique_ptr<INode> cloneUnary() const {
    return std::make_unique<T>(
        dynamic_cast<INode *>(m_right.get())->cloneAs<Right>());
  }
};

/**
 * A templte for lists of nodes.
 */
template <class Item, class Base>
class TList : public Base {
 protected:
  std::vector<std::unique_ptr<Item>> m_list{};

 public:
  TList() = default;
  void append(std::unique_ptr<Item> item) { m_list.push_back(std::move(item)); }
  std::vector<std::unique_ptr<Item>> &list() { return m_list; };
  template <class Arg1, class... Args>
  explicit TList(Arg1 arg1, Args... args) {
    m_list.push_back(move(arg1));
    // trick to get variadic template to pushback
    int dummy[1 + sizeof...(Args)] = {0, (m_list.push_back(move(args)), 0)...};
    (void)dummy;
  }
  std::vector<INode *> getChildren() override {
    std::vector<INode *> v;
    for (auto &c : m_list) {
      v.push_back(c.get());
    }
    return v;
  }
  template <class T>
  std::unique_ptr<INode> cloneList() const {
    auto res = std::make_unique<T>();
    for (auto &item : m_list) {
      auto p = static_cast<INode *>(item.get())->cloneAs<Item>();
      res->m_list.push_back(move(p));
    }
    return std::move(res);
  }
};

/**
 * NODE TYPES
 *
 * Putting the base classes here allows
 * eliminating most cross dependencies that
 * only rely on the base type.
 */

namespace condition {
/**
 * The base class from which all logical
 * conditions must derive.
 */
class Base : public INode {};
}  // namespace condition

namespace element {
class Base : public INode {};
}  // namespace element

namespace equation {
class Base : public INode {};
}  // namespace equation

namespace expression {
/**
 * The base expression class.
 */
class Base : public INode {};
}  // namespace expression

namespace model {
/**
 * A high level node that doesn't fit into any other category
 */
class Base : public INode {};
}  // namespace model

namespace statement {
/**
 * Base statment type.
 */
class Base : public INode {};
}  // namespace statement

}  // namespace cymoca::ast

namespace std {
template <>
struct hash<cymoca::ast::INode> {
  size_t operator()(const cymoca::ast::INode &obj) const {
    return hash<int>()(size_t(&obj));
  }
};
}  // namespace std
#endif
