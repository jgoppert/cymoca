//
// Created by jgoppert on 6/28/18.
//

#pragma once

//
// Created by jgoppert on 6/27/18.
//

#include <memory>
#include <vector>
#include <cassert>

#include "Listener.h"
#include "cymoca_compiler/util.h"

using namespace std;

namespace cymoca {
namespace ast {

/*
 * To add a new new node type:
 *
 * 1. Add forward declaration of node class.
 * 2. Use listener macro to declare callback.
 * 3. Declare node class. Implement required node functions.
 */

#define NODE_MACRO(NAME) \
void enter(Listener & listener) const override { listener.enter(*this); } \
void exit(Listener & listener) const override { listener.exit(*this); }

/**
 * An abstract base class for an AST node.
 */
class Node {
 public:
  Node(const type_info &type);
  virtual ~Node() = default;
  // delete copy and deep copy with clone to avoid implicit copying
  Node(const Node &other) = delete;
  Node &operator=(const Node &other) = delete;
  // accessors
  void setParent(Node *node) { _parent = node; }
  Node *getParent() const { return _parent; }
  const type_info &getType() const { return _type; }
  /**
   * Get a vector of Node *'s to all children of the Node,
   * this is used for tree traversal.
   * @return Vector of Node * to all children.
   */
  virtual vector<Node *> children() const = 0;
  /**
   * Listener callback that executes when entering a node.
   * @param listener The listener class.
   */
  virtual void exit(Listener &listener) const = 0;
  /**
 * Listener callback that executes when exiting a node.
 * @param listener The listener class.
 */
  virtual void enter(Listener &listener) const = 0;

  /**
   * Swap the old and new node. The oldNode is
   * passed by Node * since this is what the
   * listener has access to, the method must look
   * up which unique_ptr holds this child and swap it.
   * @param oldNode A pointer to the old Node.
   * @param newNode A unique_ptr to the new Node.
   */
  virtual void swapChild(Node *oldNode, unique_ptr<Node> newNode) = 0;
  /**
   * Perform a deep copy.
   * @return
   */
  virtual unique_ptr<Node> clone() const = 0;

  template<class T>
  unique_ptr<T> cloneAs() const {
    return static_unique_ptr_cast<T>(clone());
  }

 protected:
  const type_info &_type;
  Node *_parent;
};

/**
 * Abstract expression class.
 */
class Expr : public Node {
 public:
  Expr(const type_info &type) : Node(type) {}
};

/**
 * Abstract logic expression class.
 */
class LogicExpr : public Node {
 public:
  LogicExpr(const type_info &type) : Node(type) {}
};

/**
 * Abstract equation class.
 */
class Equation : public Node {
 public:
  Equation(const type_info &type) : Node(type) {}
};

/**
 * Abstract statement class.
 */
class Statement : public Node {
 public:
  Statement(const type_info &type) : Node(type) {}
};

} // ast
} // cymoca