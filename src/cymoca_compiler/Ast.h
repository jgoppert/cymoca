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

class Walker {
 public:
  Walker() {};
  virtual ~Walker() {}
  virtual void walk(Listener *listener, Node *node) const;
 protected:
  virtual void enter(Listener *listener, Node *node) const;
  virtual void exit(Listener *listener, Node *node) const;
};

class Node {
 public:
  typedef std::shared_ptr<Node> Ptr;
  Node(const std::type_info & type) : _type(type), _parent(nullptr), _children() {
  }
  virtual ~Node() {}
  virtual void enter(Listener *listener) = 0;
  virtual void exit(Listener *listener) = 0;
  void addChild(Node::Ptr child) {
    child->_parent = this;
    _children.push_back(child);
  }
  std::vector<Node::Ptr> & children() {
    return _children;
  }
  const std::type_info & getType() { return _type; }
 protected:
  const std::type_info & _type;
  Node * _parent;  // don't own parent, std pointer
  std::vector<Node::Ptr> _children;
};

// macro to define listener hooks
#define NODE(name) \
void enter(Listener * listener) override { \
  listener->enter(this); \
} \
void exit(Listener * listener) override { \
  listener->exit(this); \
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
  Equation(Expr::Ptr left, Expr::Ptr right) : Node(typeid(*this)), _left(left), _right(right) {
    assert(left != nullptr);
    assert(right != nullptr);
    addChild(left);
    addChild(right);
  };
  virtual ~Equation() {};
  Expr::Ptr left() { return _left; }
  Expr::Ptr right() { return _right; }
 private:
  Expr::Ptr _left;
  Expr::Ptr _right;
};

class EquationList : public Node {
 public:
  NODE(EquationList)
  EquationList() : Node(typeid(*this)), _equations() {
  }
  void addEquation(Node::Ptr eq) {
    addChild(eq);
    _equations.push_back(eq);
  }
  std::vector<Node::Ptr> equations() { return _equations; }
 protected:
  std::vector<Node::Ptr> _equations;
};

class IfEquation : public Node {
 public:
  NODE(IfEquation)
  IfEquation() : Node(typeid(*this)), _conditions(), _equationLists() {
  }
  virtual ~IfEquation() {};
  void addBlock(Expr::Ptr conditon, EquationList::Ptr eqs) {
    _conditions.push_back(conditon);
    _equationLists.push_back(eqs);
    addChild(conditon);
    addChild(eqs);
  }
  const std::vector<Expr::Ptr> & condition() {
    return _conditions;
  }
  const std::vector<EquationList::Ptr> & equations() {
    return _equationLists;
  }
 private:
  std::vector<Expr::Ptr> _conditions;
  std::vector<EquationList::Ptr> _equationLists;
};

class WhenEquation : public Node {
 public:
  NODE(WhenEquation)
  WhenEquation() : Node(typeid(*this)), _conditions(), _equationLists() {
  }
  virtual ~WhenEquation() {};
  void addBlock(Expr::Ptr conditon, EquationList::Ptr eqs) {
    _conditions.push_back(conditon);
    _equationLists.push_back(eqs);
    addChild(conditon);
    addChild(eqs);
  }
  const std::vector<Expr::Ptr> & condition() {
    return _conditions;
  }
  const std::vector<EquationList::Ptr> & equations() {
    return _equationLists;
  }
 private:
  std::vector<Expr::Ptr> _conditions;
  std::vector<EquationList::Ptr> _equationLists;
};

class Derivative : public Expr {
 public:
  NODE(Derivative)
  Derivative(Expr::Ptr var) : Expr(typeid(*this)), _var(var) {
    addChild(var);
  }
  virtual ~Derivative() {};
  Expr::Ptr var() {
    return _var;
  }
 private:
  Expr::Ptr _var;
};

class Pre : public Expr {
 public:
  NODE(Pre)
  Pre(Expr::Ptr var) : Expr(typeid(*this)), _var(var) {
    addChild(var);
  }
  virtual ~Pre() {};
  Expr::Ptr var() {
    return _var;
  }
 private:
  Expr::Ptr _var;
};

class Relation : public Expr {
 public:
  NODE(Relation)
  Relation(Expr::Ptr left, const std::string & op, Expr::Ptr right) :
      Expr(typeid(*this)), _left(left), _op(op), _right(right) {
    addChild(left);
    addChild(right);
  }
  virtual ~Relation() {};
  Expr::Ptr left() { return _left; }
  Expr::Ptr right() { return _right; }
  const std::string & op() { return _op; }
 private:
  Expr::Ptr _left;
  std::string _op;
  Expr::Ptr _right;
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
 private:
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
 private:
  bool _val;
};

class Negative : public Expr {
 public:
  NODE(Negative)
  Negative(Expr::Ptr e) :
      Expr(typeid(*this)), _expr(e) {
    addChild(_expr);
  }
  Expr::Ptr expr() {
    return _expr;
  }
 private:
  Expr::Ptr _expr;
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
 private:
  std::string _name;
};

class Component : public Node {
 public:
  NODE(Component)
  Component(const std::string &name) :
      Node(typeid(*this)), _name(name) {
  }
 private:
  std::string _name;
};

class Class : public Node {
 public:
  NODE(Class)
  Class() : Node(typeid(*this)) {}
  std::map<std::string, Component> _components;
  void addEquationSection(EquationList::Ptr & eqList) {
    _equationSection.push_back(eqList);
    addChild(eqList);
  }
  std::vector<EquationList::Ptr> equationSection() {
    return _equationSection;
  }
 private:
  std::vector<EquationList::Ptr> _equationSection;
};

}

}
