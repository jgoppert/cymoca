//
// Created by jgoppert on 6/22/18.
//

#pragma once

#include <string>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <assert.h>
#include <typeinfo>
#include <typeindex>

namespace cymoca {

namespace ast {

// forward declarations for listener
class Boolean;
class Class;
class ComponentRef;
class Component;
class Derivative;
class Expr;
class Equation;
class EquationBlock;
class EquationList;
class IfEquation;
class Listener;
class Negative;
class Node;
class Pre;
class Relation;
class UnsignedNumber;
class WhenEquation;


class Listener {
 public:
  virtual ~Listener() {};
  // for every node
  virtual void enterEvery(Node *ctx) {};
  virtual void exitEvery(Node *ctx) {};
  // for specific nodes
  virtual void enter(Boolean *ctx) {};
  virtual void exit(Boolean *ctx) {};
  virtual void enter(Component *ctx) {};
  virtual void exit(Component *ctx) {};
  virtual void enter(Class *ctx) {};
  virtual void exit(Class *ctx) {};
  virtual void enter(ComponentRef *ctx) {};
  virtual void exit(ComponentRef *ctx) {};
  virtual void enter(Derivative *ctx) {};
  virtual void exit(Derivative *ctx) {};
  virtual void enter(Equation *ctx) {};
  virtual void exit(Equation *ctx) {};
  virtual void enter(EquationBlock *ctx) {};
  virtual void exit(EquationBlock *ctx) {};
  virtual void enter(EquationList *ctx) {};
  virtual void exit(EquationList *ctx) {};
  virtual void enter(Expr *ctx) {};
  virtual void exit(Expr *ctx) {};
  virtual void enter(IfEquation *ctx) {};
  virtual void exit(IfEquation *ctx) {};
  virtual void enter(Negative *ctx) {};
  virtual void exit(Negative *ctx) {};
  virtual void enter(Pre *ctx) {};
  virtual void exit(Pre *ctx) {};
  virtual void enter(Relation *ctx) {};
  virtual void exit(Relation *ctx) {};
  virtual void enter(UnsignedNumber *ctx) {};
  virtual void exit(UnsignedNumber *ctx) {};
  virtual void enter(WhenEquation *ctx) {};
  virtual void exit(WhenEquation *ctx) {};
};

class Node {
 public:
  typedef std::shared_ptr<Node> Ptr;
  Node(const std::type_info & type) : _type(type), _parent(nullptr), _children() {
  }
  virtual ~Node() {}
  virtual void enter(Listener &listener) = 0;
  virtual void exit(Listener &listener) = 0;
  void _addChild(Node::Ptr child) {
    assert(child.get() != nullptr);
    child->_parent = this;
    _children.push_back(child);
  }
  std::vector<Node::Ptr> & children() {
    return _children;
  }
  const std::type_info & getType() { return _type; }
  Node(const cymoca::ast::Node&) = delete;
  void operator=(const cymoca::ast::Node&) = delete;
  Node * parent() { return _parent; }
  void swapChild(Node * original, Node::Ptr replacement) {
    for (auto &c: _children) {
      if (original == c.get()) {
        Node * newData = replacement.get();
        c.swap(replacement);
        assert(c.get() == newData);
      }
    }
  }
 protected:
  template <typename T>
  std::shared_ptr<T> _childAs(size_t i) {
    auto e = std::dynamic_pointer_cast<T>(_children[i]);
    assert(e.get() != nullptr);
    return e;
  }
  Node::Ptr _child(size_t i) {
    assert(i < _children.size());
    return _children[i];
  }
  const std::type_info & _type;
  Node * _parent;  // don't own parent, std pointer
  std::vector<Node::Ptr> _children;
};

// macro to define listener hooks
#define NODE(name) \
void enter(Listener & listener) override { \
  listener.enter(this); \
} \
void exit(Listener & listener) override { \
  listener.exit(this); \
} \
typedef std::shared_ptr<name> Ptr;

// abstract class to group mathematical expressions
class Expr : public Node {
 public:
  NODE(Expr)
  Expr(const std::type_info & type) : Node(typeid(*this)) {
  }
  virtual ~Expr() {};
};

class Equation : public Node {
 public:
  NODE(Equation)
  Equation(Expr::Ptr left, Expr::Ptr right) : Node(typeid(*this)) {
    _addChild(left);
    _addChild(right);
  };
  virtual ~Equation() {};
  Expr::Ptr left() { return _childAs<Expr>(0); }
  Expr::Ptr right() { return _childAs<Expr>(1);  }
};

class EquationList : public Node {
 public:
  NODE(EquationList)
  EquationList() : Node(typeid(*this)) {
  }
  virtual ~EquationList() {};
  void addEquation(Node::Ptr eq) {
    _addChild(eq);
  }
  std::vector<Node::Ptr> equations() { return _children; }
};

class EquationBlock : public Node {
 public:
  NODE(EquationBlock)
  EquationBlock(Expr::Ptr condition, EquationList::Ptr eqs) : Node(typeid(*this)) {
    _addChild(condition);
    _addChild(eqs);
  }
  virtual ~EquationBlock() {};
  const Expr::Ptr condition() {
    return _childAs<Expr>(0);
  }
  const EquationList::Ptr equations() {
    return _childAs<EquationList>(1);
  }
};


class IfEquation : public Node {
 public:
  NODE(IfEquation)
  IfEquation() : Node(typeid(*this)) {
  }
  virtual ~IfEquation() {};
  void addBlock(EquationBlock::Ptr block) {
    _addChild(block);
  }
  std::vector<EquationBlock::Ptr> blocks() {
    std::vector<EquationBlock::Ptr> r;
    for (size_t i=0; i<_children.size(); i++) {
      r.push_back(_childAs<EquationBlock>(i));
    }
    return r;
  }
};

class WhenEquation : public Node {
 public:
  NODE(WhenEquation)
  WhenEquation() : Node(typeid(*this)) {
  }
  virtual ~WhenEquation() {};
  void addBlock(Expr::Ptr condition, EquationList::Ptr eqs) {
    _addChild(std::make_shared<EquationBlock>(condition, eqs));
  }
  std::vector<EquationBlock::Ptr> blocks() {
    std::vector<EquationBlock::Ptr> r;
    for (size_t i=0; i<_children.size(); i++) {
      r.push_back(_childAs<EquationBlock>(i));
    }
    return r;
  }
};

class Derivative : public Expr {
 public:
  NODE(Derivative)
  Derivative(Expr::Ptr var) : Expr(typeid(*this)) {
    _addChild(var);
  }
  virtual ~Derivative() {};
  Expr::Ptr var() {
    return _childAs<Expr>(0);
  }
};

class Pre : public Expr {
 public:
  NODE(Pre)
  Pre(Expr::Ptr var) : Expr(typeid(*this)) {
    _addChild(var);
  }
  virtual ~Pre() {};
  Expr::Ptr var() {
    return _childAs<Expr>(0);
  }
};

class Relation : public Expr {
 public:
  NODE(Relation)
  Relation(Expr::Ptr left, const std::string & op, Expr::Ptr right) :
      Expr(typeid(*this)), _op(op) {
    _addChild(left);
    _addChild(right);
  }
  virtual ~Relation() {};
  Expr::Ptr left() { return _childAs<Expr>(0); }
  Expr::Ptr right() { return _childAs<Expr>(1);  }
  const std::string & op() { return _op; }
 protected:
  std::string _op;
};


class UnsignedNumber : public Expr {
 public:
  NODE(UnsignedNumber)
  UnsignedNumber(double val) : Expr(typeid(*this)), _val(val) {
  }
  virtual ~UnsignedNumber() {};
  double val() {
    return _val;
  }
 protected:
  double _val;
};


class Boolean : public Expr {
 public:
  NODE(Boolean)
  Boolean(bool val) : Expr(typeid(*this)), _val(val) {
  }
  virtual ~Boolean() {};
  bool val() {
    return _val;
  }
 protected:
  bool _val;
};

class Negative : public Expr {
 public:
  NODE(Negative)
  Negative(Expr::Ptr e) :
      Expr(typeid(*this)) {
    _addChild(e);
  }
  Expr::Ptr expr() {
    return _childAs<Expr>(0);
  }
};

class ComponentRef : public Expr {
 public:
  NODE(ComponentRef)
  ComponentRef(const std::string &name) :
      Expr(typeid(*this)), _name(name) {
  }
  std::string name() {
    return _name;
  }
 protected:
  std::string _name;
};

class Component : public Node {
 public:
  NODE(Component)
  Component(const std::string &name) :
      Node(typeid(*this)), _name(name) {
  }
 protected:
  std::string _name;
};

class Class : public Node {
 public:
  NODE(Class)
  Class() : Node(typeid(*this)) {}
  void addEquationSection(EquationList::Ptr & eqList) {
    _addChild(eqList);
  }
  std::vector<EquationList::Ptr> equationSections() {
    std::vector<EquationList::Ptr> r;
    for (size_t i=0; i<_children.size(); i++) {
      r.push_back(_childAs<EquationList>(i));
    }
    return r;
  }
};

class Walker {
 public:
  Walker() {};
  virtual ~Walker() {}
  virtual void walk(Listener &listener, Node::Ptr node) const;
 protected:
  virtual void enter(Listener &listener, Node::Ptr node) const;
  virtual void exit(Listener &listener, Node::Ptr node) const;
};

}

}
