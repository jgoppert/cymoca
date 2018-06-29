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
  virtual ~Walker() = default;

  /**
   * Walk a tree.
   * @param node The node to walk recursively.
   * @param listener The listener.
   */
  void walk(Node &node, Listener &listener);

  /**
   * Enter routine, if you want to change the order
   * of the calls, you can override this.
   * @param node The current node.
   * @param listener The listener.
   */
  virtual void enter(Node &node, Listener & listener);

  /**
   * Exit routine, if you want to change the order
   * of the calls, you can override this.
   * @param node The current node.
   * @param listener The listener.
   */
  virtual void exit(Node &node, Listener & listener);
};

} // ast
} // cymoca