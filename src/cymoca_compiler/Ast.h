//
// Created by jgoppert on 6/22/18.
//

#ifndef CYMOCA_AST_H
#define CYMOCA_AST_H

#include <string>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <assert.h>

namespace cymoca {

namespace ast {

// forward declarations for listener
class UnsignedNumber;
class Equation;
class Negative;
class ComponentRef;
class Component;
class Class;
class Node;
class Derivative;
class Listener;
class Relation;
class WhenEquation;

class Listener {
 public:
  virtual void enterEvery(Node *ctx) {};
  virtual void exitEvery(Node *ctx) {};
  virtual void enter(UnsignedNumber *ctx) {};
  virtual void exit(UnsignedNumber *ctx) {};
  virtual void enter(Equation *ctx) {};
  virtual void exit(Equation *ctx) {};
  virtual void enter(Negative *ctx) {};
  virtual void exit(Negative *ctx) {};
  virtual void enter(Component *ctx) {};
  virtual void exit(Component *ctx) {};
  virtual void enter(Class *ctx) {};
  virtual void exit(Class *ctx) {};
  virtual void enter(ComponentRef *ctx) {};
  virtual void exit(ComponentRef *ctx) {};
  virtual void enter(Derivative *ctx) {};
  virtual void exit(Derivative *ctx) {};
  virtual void enter(Relation *ctx) {};
  virtual void exit(Relation *ctx) {};
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
  Node() : _parent(nullptr), _children() {
  }
  virtual ~Node() {}
  virtual void enter(Listener *listener) = 0;
  virtual void exit(Listener *listener) = 0;
  void addChild(Node * child) {
    child->_parent = this;
    _children.push_back(child);
  }
  std::vector<Node *> & children() {
    return _children;
  }
 protected:
  Node * _parent;
  std::vector<Node *> _children;
};

// macro to define listener hooks
#define NODE \
void enter(Listener * listener) override { \
  listener->enter(this); \
} \
void exit(Listener * listener) override { \
  listener->exit(this); \
}

class Expr : public Node {
 public:
  Expr() : Node() {
  }
  virtual ~Expr() {};
};

class WhenEquation : public Expr {
 public:
  NODE
  WhenEquation(Expr * cond) : Expr(), _cond(cond) {
    addChild(cond);
  }
  virtual ~WhenEquation() {};
  Node * cond() {
    return _cond;
  }
 private:
  Node * _cond;
};

class Derivative : public Expr {
 public:
  NODE
  Derivative(Expr * var) : Expr(), _var(var) {
    addChild(var);
  }
  virtual ~Derivative() {};
  Node * var() {
    return _var;
  }
 private:
  Node * _var;
};

class Relation : public Expr {
 public:
  NODE
  Relation(Node * left, const std::string & op, Node * right) :
      Expr(), _left(left), _op(op), _right(right) {
    addChild(left);
    addChild(right);
  }
  virtual ~Relation() {};
  Node * left() { return _left; }
  Node * right() { return _right; }
  const std::string & op() { return _op; }
 private:
  Node * _left;
  std::string _op;
  Node * _right;
};


class UnsignedNumber : public Expr {
 public:
  NODE
  UnsignedNumber(double val) : Expr(), _val(val) {
  }
  virtual ~UnsignedNumber() {};
  double val() {
    return _val;
  }
 private:
  double _val;
};

class Equation : public Expr {
 public:
  NODE
  Equation(Expr * left, Expr * right) : Expr(), _left(left), _right(right) {
    assert(left != nullptr);
    assert(right != nullptr);
    addChild(left);
    addChild(right);
  };
  virtual ~Equation() {};
  Expr * left() { return _left; }
  Expr * right() { return _right; }
 private:
  Expr * _left;
  Expr * _right;
};

class Negative : public Expr {
 public:
  NODE
  Negative(Expr * e) :
      _expr(e) {
    addChild(_expr);
  }
 private:
  Expr * _expr;
};

class ComponentRef : public Expr {
 public:
  NODE
  ComponentRef(const std::string &name) :
      _name(name) {
  }
  std::string name() {
    return _name;
  }
 private:
  std::string _name;
};

class Component : public Node {
 public:
  NODE
  Component() :
      _name() {
  }
  Component(const std::string &name) :
      _name(name) {
  }
 private:
  std::string _name;
};

class Class : public Node {
 public:
  NODE
  std::map<std::string, Component> _components;
  std::list<Equation> _equations;
};

}

}

#endif //CYMOCA_AST_H
