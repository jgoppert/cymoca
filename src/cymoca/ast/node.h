#ifndef CYMOCA_AST_NODE_H_
#define CYMOCA_AST_NODE_H_

#include <cassert>
#include <memory>
#include <vector>

#include "declarations.h"
#include "listener/listener.h"

namespace cymoca::ast {

#define NODE_MACRO                                                         \
  void enter(listener::Base &listener) override { listener.enter(*this); } \
  void exit(listener::Base &listener) override { listener.exit(*this); }

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
class Binary : public Base, virtual public INode {
 protected:
  std::unique_ptr<Left> left;
  std::unique_ptr<Right> right;

 public:
  Binary(std::unique_ptr<Left> left, std::unique_ptr<Right> right)
      : left(move(left)), right(move(right)) {}
  std::vector<INode *> getChildren() override {
    return {left.get(), right.get()};
  }
  template <class T>
  std::unique_ptr<INode> cloneBinary() const {
    return std::make_unique<T>(
        dynamic_cast<INode *>(left.get())->cloneAs<Left>(),
        dynamic_cast<INode *>(right.get())->cloneAs<Right>());
  }
};

/**
 * A template for unary nodes.
 */
template <class Right, class Base>
class Unary : public Base, virtual public INode {
 protected:
  std::unique_ptr<Right> right;

 public:
  explicit Unary(std::unique_ptr<Right> right) : right(move(right)) {}
  std::vector<INode *> getChildren() override { return {right.get()}; }
  template <class T>
  std::unique_ptr<INode> cloneUnary() const {
    return std::make_unique<T>(
        dynamic_cast<INode *>(right.get())->cloneAs<Right>());
  }
};

/**
 * A templte for lists of nodes.
 */
template <class Item, class Base>
class List : public Base, virtual public INode {
 protected:
  std::vector<std::unique_ptr<Item>> list{};

 public:
  List() = default;
  template <class Arg1, class... Args>
  explicit List(Arg1 arg1, Args... args) {
    list.push_back(move(arg1));
    auto dummy = {(list.push_back(move(args)), 0)...};
    (void)dummy;
  }
  std::vector<INode *> getChildren() override {
    std::vector<INode *> v;
    for (auto &c : list) {
      v.push_back(c.get());
    }
    return v;
  }
  template <class T>
  std::unique_ptr<INode> cloneList() const {
    auto res = std::make_unique<T>();
    for (auto &item : list) {
      auto p = static_cast<INode *>(item.get())->cloneAs<Item>();
      res->list.push_back(move(p));
    }
    return std::move(res);
  }
};

}  // namespace cymoca::ast
#endif
