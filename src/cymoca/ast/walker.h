#ifndef CYMOCA_AST_WALKER_H_
#define CYMOCA_AST_WALKER_H_

#include <iostream>
#include "../util.h"
#include "listener/listener.h"
#include "node.h"

namespace cymoca::ast {

class Walker {
 public:
  Walker(bool verbose = false) : m_verbose(verbose) {}
  void walk(INode *tree, listener::Base &listener) {
    assert(tree);
    if (m_verbose) {
      std::cout << std::string(m_depth, '\t')
                << demangle(tree->getType().name()) << " {" << std::endl;
    }
    listener.enterEvery(tree, this);
    tree->enter(listener, this);
    for (auto &c : tree->getChildren()) {
      assert(c);
      m_depth += 1;
      walk(c, listener);
      m_depth -= 1;
    }
    tree->exit(listener, this);
    listener.exitEvery(tree, this);
    if (m_verbose) {
      std::cout << std::string(m_depth, '\t') << "} "
                << " end " << demangle(tree->getType().name()) << std::endl;
    }
  }
  int getDepth() { return m_depth; }

 private:
  int m_depth{0};
  bool m_verbose = {false};
};

}  // namespace cymoca::ast
#endif
