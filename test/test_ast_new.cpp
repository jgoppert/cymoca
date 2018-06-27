//
// Created by jgoppert on 6/27/18.
//

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>


using namespace std;

/*
 * To add a new new node type:
 *
 * 1. Add forward declaration of node class.
 * 2. Use listener macro to declare callback.
 * 3. Declare node class. Implement required node functions.
 */

// forward declarations
class Binary;
class Class;
class Unary;
class Number;
class Node;
class Expr;
class Equation;
class Listener;

template<class T>
class Dict;

template<class T>
class List;

#define NODE_MACRO(NAME) \
void enter(Listener & listener) const override { listener.enter(*this); } \
void exit(Listener & listener) const override { listener.exit(*this); } \
using Ptr = unique_ptr<NAME>;

#define VISITOR_MACRO(NAME) \
virtual void enter(const NAME & ctx) {}; \
virtual void exit(const NAME & ctx) {};

template<typename Derived, typename Base>
unique_ptr<Derived>
static_unique_ptr_cast(unique_ptr<Base> &&p) {
  auto d = static_cast<Derived *>(p.release());
  return unique_ptr<Derived>(d);
}

/**
 * An abstract base class for an AST node.
 */
class Node {
 public:
  using Ptr = unique_ptr<Node>;
  Node(const type_info & type) : _type(type), _parent(nullptr) {}
  virtual ~Node() = default;
  Node(const Node &other) = default;
  Node &operator=(const Node &other) = default;
  // accessors
  void setParent(Node *node) { _parent = node; }
  Node *getParent() const { return _parent; }
  const type_info & getType() { return _type; }
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
  virtual void swapChild(Node *oldNode, Node::Ptr newNode) = 0;
  /**
   * Perform a deep copy.
   * @return
   */
  virtual Node::Ptr clone() const = 0;
 protected:
  const type_info & _type;
  Node *_parent;
};

class Listener {
 public:
  Listener() = default;
  virtual ~Listener() = default;
  virtual void enterEvery(const Node &ctx) {};
  virtual void exitEvery(const Node &ctx) {};
  VISITOR_MACRO(Number)
  VISITOR_MACRO(Binary)
  VISITOR_MACRO(Class)
  VISITOR_MACRO(Equation)
  VISITOR_MACRO(Unary)
  VISITOR_MACRO(Dict<Number>)
  VISITOR_MACRO(List<Equation>)
};

class SwapListener : public Listener {
 public:
  void swap() {
    for (auto &s: _swap) {
      auto n = (Node *) (s.first);
      n->getParent()->swapChild(n, move(s.second));
    }
  }
  void setSwap(const Node & ctx, Node::Ptr node) {
    _swap[&ctx] = move(node);
  }
 private:
  map<const Node *, unique_ptr<Node>> _swap{};
};

/**
 * Abstract expression class.
 */
class Expr : public Node {
 public:
  Expr(const type_info & type) : Node(type) {}
  using Ptr = unique_ptr<Expr>;
};

class Equation : public Node {
 public:
  NODE_MACRO(Equation)
  Equation(Expr::Ptr left, Expr::Ptr right) : Node(typeid(*this)), _m(), _left(move(left)), _right(move(right)) {
    _left->setParent(this);
    _right->setParent(this);
  }
  // accessors
  const Expr &left() const { return *_left; }
  const Expr &right() const { return *_right; }
  vector<Node *> children() const override {
    return {_left.get(), _right.get()};
  }
  void swapChild(Node *oldChild, Node::Ptr newChild) override {
    if (oldChild == _left.get()) {
      auto e = static_unique_ptr_cast<Expr>(move(newChild));
      _left.swap(e);
      return;
    } else if (oldChild == _right.get()) {
      auto e = static_unique_ptr_cast<Expr>(move(newChild));
      _right.swap(e);
      return;
    }
    assert(false);
  }
  Node::Ptr clone() const override {
    return make_unique<Equation>(
        static_unique_ptr_cast<Expr>(_left->clone()),
        static_unique_ptr_cast<Expr>(_right->clone()));
  }
 protected:
  map<Node *, unique_ptr<Expr> *> _m;
  Expr::Ptr _left;
  Expr::Ptr _right;
};

class Number : public Expr {
 public:
  NODE_MACRO(Number)
  explicit Number(double val) : Expr(typeid(*this)), _val(val) {};
  // accessors
  double val() const { return _val; }
  // node interface
  vector<Node *> children() const override { return {}; }
  void swapChild(Node *oldChild, Node::Ptr newChild) override {}
  Node::Ptr clone() const override {
    return make_unique<Number>(val());
  }
 protected:
  double _val;
};

enum class BinaryOp { ADD, SUB, MUL, DIV };
map<BinaryOp, string> binaryOpStr = {
    {BinaryOp::ADD, "+"},
    {BinaryOp::SUB, "-"},
    {BinaryOp::MUL, "*"},
    {BinaryOp::DIV, "/"}
};

class Binary : public Expr {
 public:
  NODE_MACRO(Binary)
  Binary(Expr::Ptr left, BinaryOp op, Expr::Ptr right) :
      Expr(typeid(*this)), _left(move(left)), _op(op), _right(move(right)) {
    _left->setParent(this);
    _right->setParent(this);
  }
  // accessors
  const Expr &left() const { return *_left; }
  const Expr &right() const { return *_right; }
  BinaryOp op() const { return _op; }
  // node interface
  vector<Node *> children() const override {
    return {_left.get(), _right.get()};
  }
  void swapChild(Node *oldChild, Node::Ptr newChild) override {
    if (oldChild == _left.get()) {
      auto e = static_unique_ptr_cast<Expr>(move(newChild));
      _left.swap(e);
    } else if (oldChild == _right.get()) {
      auto e = static_unique_ptr_cast<Expr>(move(newChild));
      _right.swap(e);
    } else{
      assert(false);
    }
  }
  Node::Ptr clone() const override {
    return make_unique<Binary>(
        static_unique_ptr_cast<Expr>(_left->clone()),
        op(),
        static_unique_ptr_cast<Expr>(_right->clone()));
  }
 protected:
  Expr::Ptr _left;
  BinaryOp _op;
  Expr::Ptr _right;
};

enum class UnaryOp { NEG };
map<UnaryOp, string> unaryOpStr = {
    {UnaryOp::NEG, "-"}
};

class Unary : public Expr {
 public:
  NODE_MACRO(Unary)
  Unary(UnaryOp op, Expr::Ptr right) :
      Expr(typeid(*this)), _op(op), _right(move(right)) {
    _right->setParent(this);
  }
  // accessors
  UnaryOp op() const { return _op; }
  const Expr &right() const { return *_right; }
  // node interface
  vector<Node *> children() const override {
    return {_right.get()};
  }
  void swapChild(Node *oldChild, Node::Ptr newChild) override {
    if (oldChild == _right.get()) {
      auto e = static_unique_ptr_cast<Expr>(move(newChild));
      _right.swap(e);
      return;
    }
    assert(false);
  }
  Node::Ptr clone() const override {
    return make_unique<Unary>(
        op(), static_unique_ptr_cast<Expr>(_right->clone()));
  }
 protected:
  UnaryOp _op;
  Expr::Ptr _right;
};

template<class T>
class Dict : public Node {
 public:
  NODE_MACRO(Dict)
  Dict() : Node(typeid(*this)), _map() {}
  // accessors
  // node interface
  void set(const string & name, unique_ptr<T> val) {
    val->setParent(this);
    _map[name] = move(val);
  }
  T & get(const string & name) {
    T & v =  *_map[name];
    assert(&v);
    return v;
  }
  vector<Node *> children() const override {
    vector<Node *> v;
    for (auto &c: _map) {
      v.push_back(c.second.get());
    }
    return v;
  }
  void swapChild(Node *oldChild, Node::Ptr newChild) override {
    // TODO: doing a linear search to find the right pointer, slow
    for (auto &c: _map) {
      if (c.second.get() == oldChild) {
        auto e = static_unique_ptr_cast<T>(move(newChild));
        c.second.swap(e);
        return;
      }
    }
    assert(false);
  }
  Node::Ptr clone() const override {
    auto newDict = make_unique<Dict>();
    for (auto &c: _map) {
      auto e = static_unique_ptr_cast<T>(move(c.second->clone()));
      newDict->set(c.first, move(e));
    }
    return move(newDict);
  }
 protected:
  map<string , unique_ptr<T>> _map;
};


template<class T>
class List : public Node {
 public:
  NODE_MACRO(List)
  List() : Node(typeid(*this)), _list() {}
  // accessors
  // node interface
  void append(unique_ptr<T> val) {
    val->setParent(this);
    _list.push_back(move(val));
  }
  vector<unique_ptr<T>> get() {
    return _list;
  }
  vector<Node *> children() const override {
    vector<Node *> v;
    for (auto &c: _list) {
      v.push_back(c.get());
    }
    return v;
  }
  void swapChild(Node *oldChild, Node::Ptr newChild) override {
    // TODO: doing a linear search to find the right pointer, slow
    for (auto &c: _list) {
      if (c.get() == oldChild) {
        auto e = static_unique_ptr_cast<T>(move(newChild));
        c.swap(e);
        return;
      }
    }
    assert(false);
  }
  Node::Ptr clone() const override {
    return make_unique<List>();
  }
 protected:
  vector<unique_ptr<T>> _list;
};

class Class : public Node {
 public:
  NODE_MACRO(Class)
  Class() : Node(typeid(*this)), _components(), _equations() {
    _components.setParent(this);
    _equations.setParent(this);
  }
  // accessors
  Dict<Number> & components() { return _components; };
  List<Equation> & equations() { return _equations; };
  // node interface
  vector<Node *> children() const override {
    return {
        (Node *)(&_components),
        (Node *)(&_equations)
    };
  }
  void swapChild(Node *oldChild, Node::Ptr newChild) override {
    // TODO
  }
  Node::Ptr clone() const override {
    // TODO
    return make_unique<Class>();
  }
 protected:
  // no need for unique_ptr here, we know the type
  Dict<Number> _components;
  List<Equation> _equations;
};

/**
 * A function to walk a tree.
 * @param node The root node to start walking at.
 * @param listener The listener.
 */
void walk(Node &node, Listener &listener) {
  listener.enterEvery(node);
  node.enter(listener);
  for (auto &c: node.children()) {
    walk(*c, listener);
  }
  node.exit(listener);
  listener.exitEvery(node);
}

/**
 * A listener to print to quasi-lisp format.
 */
class LispPrinter : public Listener {
 protected:
  stringstream _ss;
 public:
  LispPrinter() : _ss("") {}
  void enterEvery(const Node &ctx) override {
    _ss << "(";
  }
  void exitEvery(const Node &ctx) override {
    _ss << ")";
  }
  void enter(const Class &ctx) override {
    _ss << "class";
  }
  void enter(const Dict<Number> &ctx) override {
    _ss << "dict of num";
  }
  void enter(const List<Equation> &ctx) override {
    _ss << "list of eq";
  }
  void enter(const Number &ctx) override {
    _ss << ctx.val();
  }
  void enter(const Unary &ctx) override {
    _ss << unaryOpStr[ctx.op()];
  }
  void enter(const Binary &ctx) override {
    _ss << binaryOpStr[ctx.op()];
  }
  string get() {
    string s = _ss.str();
    _ss.str("");
    return s;
  }
};

/**
 * A listener to add one to every number.
 */
class AddOne : public SwapListener {
  void enter(const Number &ctx) override {
    setSwap(ctx, make_unique<Number>(ctx.val() + 1));
  }
};

int main() {
  auto e1 = make_unique<Binary>(
      make_unique<Number>(1),
      BinaryOp::SUB,
      make_unique<Number>(1));

  LispPrinter printer;
  walk(*e1, printer);
  cout << printer.get() << endl;

  auto e1Copy = e1->clone();

  AddOne addListener;
  walk(*e1, addListener);
  addListener.swap();
  walk(*e1, printer);
  cout << "rewrite:" << printer.get() << endl;

  walk(*e1Copy, printer);
  cout << "original:" << printer.get() << endl;

  auto e2 = make_unique<Equation>(
      make_unique<Unary>(
          UnaryOp::NEG,
          make_unique<Number>(1)
      ),
      make_unique<Number>(2));
  walk(*e2, printer);
  cout << "unary:" << printer.get() << endl;

  auto e3 = make_unique<Dict<Number>>();
  e3->set("hi", make_unique<Number>(2));

  walk(*e3, printer);
  cout << "dict:" << printer.get() << endl;

  cout << "hi val: " << e3->get("hi").val() << endl;

  auto e4 = static_unique_ptr_cast<Dict<Number>>(move(e3->clone()));

  cout << "hi val: " << e4->get("hi").val() << endl;

  auto e5 = make_unique<Class>();
  e5->components().set("hi", make_unique<Number>(2));
  e5->components().set("bob", make_unique<Number>(3));
  e5->components().set("steve", make_unique<Number>(4));
  e5->equations().append(make_unique<Equation>(
      make_unique<Number>(99),
      make_unique<Number>(98)
  ));

  cout << "class hi: " << e5->components().get("hi").val()  << endl;
  cout << "class bob: " << e5->components().get("bob").val()  << endl;
  cout << "class steve: " << e5->components().get("steve").val()  << endl;

  walk(*e5, printer);
  cout << "class:" << printer.get() << endl;

  auto e6 = e5->clone();
  walk(*e6, printer);
  cout << "class:" << printer.get() << endl;

  // this fails due to type checking
  /*
  auto sum2 = make_unique<Binary>(
      make_unique<Equation>(
          make_unique<Number>(1),
          make_unique<Number>(2)),
      Binary::Op::SUB,
      make_unique<Number>(1));
  */
}