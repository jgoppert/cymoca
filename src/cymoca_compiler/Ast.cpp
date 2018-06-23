//
// Created by jgoppert on 6/22/18.
//

#include "Ast.h"

namespace cymoca {

namespace ast {

void Walker::walk(Listener *listener, Node *t) const {
  enter(listener, t);
  for (auto child : t->children()) {
    walk(listener, child);
  }
  exit(listener, t);
}

void Walker::enter(Listener *listener, Node *node) const {
  listener->enterEvery(node);
  node->enter(listener);
}

void Walker::exit(Listener *listener, Node *node) const {
  node->exit(listener);
  listener->exitEvery(node);
}


}
}