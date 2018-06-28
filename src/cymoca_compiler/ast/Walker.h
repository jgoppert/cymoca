//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Node.h"
#include "Listener.h"

namespace cymoca {
namespace ast {

class Walker {
 public:
  /**
   * Walk a tree.
   * @param node The root node to start walking at.
   * @param listener The listener.
   */
  void walk(Node &node, Listener &listener);
};

} // ast
} // cymoca