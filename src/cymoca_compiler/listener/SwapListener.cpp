//
// Created by jgoppert on 6/28/18.
//

#include "cymoca_compiler/listener/SwapListener.h"
#include "cymoca_compiler/ast/Node.h"

namespace cymoca {
namespace listener {

void SwapListener::apply() {
  for (auto &s: _swap) {
    auto n = (Node *) (s.first);
    n->parent()->swapChild(n, move(s.second));
  }
  _swap.clear();
}

void SwapListener::set(const Node &ctx, unique_ptr<Node> node) {
  _swap[&ctx] = move(node);
}

} // listener
} // cymoca
