//
// Created by jgoppert on 6/28/18.
//

#include "Walker.h"

namespace cymoca {
namespace ast {

void Walker::walk(Node &node, Listener &listener) {
  enter(node, listener);
  for (auto &c: node.children()) {
    walk(*c, listener);
  }
  exit(node, listener);
}


void Walker::enter(Node &node, Listener & listener) {
  listener.enterEvery(node);
  node.enter(listener);
}

void Walker::exit(Node &node, Listener & listener) {
  node.exit(listener);
  listener.exitEvery(node);
}

} // ast
} // cymoca