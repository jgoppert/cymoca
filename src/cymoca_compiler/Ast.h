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
class Listener;

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
  Node() : children() {
  }
  virtual ~Node() {}
  virtual void enter(Listener *listener) = 0;
  virtual void exit(Listener *listener) = 0;
  void addChild(std::shared_ptr<Node> child) {
    children.push_back(child);
  }
  std::vector<std::shared_ptr<Node>> children;
};

// macro to define listener hooks
#define NODE \
void enter(Listener * listener) override { \
  listener->enter(this); \
} \
void exit(Listener * listener) override { \
  listener->exit(this); \
}

class UnsignedNumber : public Node {
 public:
  NODE
  UnsignedNumber(double val) : Node(), _val(val) {
  }
  virtual ~UnsignedNumber() {};
  double val() {
    return _val;
  }
 private:
  double _val;
};

class Equation : public Node {
 public:
  NODE
  Equation(Node * left, Node * right) : Node() {};
  virtual ~Equation() {};
  Node * left() { return _left; }
  Node * right() { return _right; }
 private:
  Node * _left;
  Node * _right;
};

class Negative : public Node {
 public:
  NODE
  Negative(std::shared_ptr<Node> e) :
      _expr(e) {
    addChild(_expr);
  }
 private:
  std::shared_ptr<Node> _expr;
};

class ComponentRef : public Node {
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
