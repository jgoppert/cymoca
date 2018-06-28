//
// Created by jgoppert on 6/28/18.
//

#include "Walker.h"

namespace cymoca {
namespace ast {

void Walker::walk(Node &node, Listener &listener) {
  listener.enterEvery(node);
  node.enter(listener);
  for (auto &c: node.children()) {
    walk(*c, listener);
  }
  node.exit(listener);
  listener.exitEvery(node);
}

} // ast
} // cymoca