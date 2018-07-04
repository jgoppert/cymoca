//
// Created by jgoppert on 6/28/18.
//

#pragma once

#include "Listener.h"
#include "Node.h"

namespace cymoca::ast {

class Walker {
public:
  virtual ~Walker() = default;

  /**
   * Walk a tree.
   * @param node The node to walk recursively.
   * @param listener The listener.
   */
  void walk(Node &node, Listener &listener);
  void walk(const Node &node, ConstListener &listener);

  /**
   * Enter routine, if you want to change the order
   * of the calls, you can override this.
   * @param node The current node.
   * @param listener The listener.
   */
  virtual void enter(Node &node, Listener &listener);
  virtual void enter(const Node &node, ConstListener &listener);

  /**
   * Exit routine, if you want to change the order
   * of the calls, you can override this.
   * @param node The current node.
   * @param listener The listener.
   */
  virtual void exit(Node &node, Listener &listener);
  virtual void exit(const Node &node, ConstListener &listener);
};

} // namespace cymoca::ast

// vim: set et fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
