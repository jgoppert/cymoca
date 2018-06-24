//
// Created by jgoppert on 6/22/18.
//

#include "Ast.h"

namespace cymoca {

namespace ast {

void Walker::walk(Listener &listener, Node::Ptr node) const {
  enter(listener, node);
  for (auto &child : node->children()) {
    walk(listener, child);
  }
  exit(listener, node);
}

void Walker::enter(Listener &listener, Node::Ptr node) const {
  listener.enterEvery(node.get());
  node->enter(listener);
}

void Walker::exit(Listener &listener, Node::Ptr node) const {
  node->exit(listener);
  listener.exitEvery(node.get());
}


}
}