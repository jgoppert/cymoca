#ifndef CYMOCA_AST_WALKER_H_
#define CYMOCA_AST_WALKER_H_

#include "listener/listener.h"
#include "node.h"

namespace cymoca::ast {

class Walker {
 public:
  void walk(INode &tree, listener::Base &listener) {
    listener.enterEvery(tree);
    tree.enter(listener);
    for (auto &c : tree.getChildren()) {
      walk(*c, listener);
    }
    tree.exit(listener);
    listener.exitEvery(tree);
  }
};

}  // namespace cymoca::ast
#endif
