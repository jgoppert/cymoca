//
// Created by jgoppert on 6/28/18.
//

#include "SwapListener.h"
#include "Node.h"

namespace cymoca {
namespace ast {

void SwapListener::swap() {
  for (auto &s: _swap) {
    auto n = (Node *) (s.first);
    n->getParent()->swapChild(n, move(s.second));
  }
}

void SwapListener::setSwap(const Node &ctx, unique_ptr<Node> node) {
  _swap[&ctx] = move(node);
}

} // ast
} // cymoca