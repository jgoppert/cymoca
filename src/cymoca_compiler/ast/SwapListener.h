//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Listener.h"
#include <memory>
#include <map>

using namespace std;

namespace cymoca {
namespace ast {

class SwapListener : public Listener {
 public:
  void swap();
  void setSwap(const Node &ctx, unique_ptr<Node> node);
 private:
  map<const Node *, unique_ptr<Node>> _swap{};
};

} // ast
} // cymoca