#ifndef CYMOCA_AST_STATEMENT_STATEMENT_H_
#define CYMOCA_AST_STATEMENT_STATEMENT_H_

#include "../node.h"

namespace cymoca::ast::statement {

/**
 * A list of statements
 */
class List : public TList<Base, Base> {
 public:
  NODE_MACRO
  using TList::TList;
  std::unique_ptr<INode> clone() const override { return cloneList<List>(); }
};

/**
 * A block of statements with a condition
 * controlling the block execution.
 */
class Block : public Base {
 protected:
  std::unique_ptr<condition::Base> m_condition{};
  std::unique_ptr<List> m_statements{};

 public:
  NODE_MACRO
};

/**
 * An if statement, composed of a List of Blocks.
 */
class If : public TList<Block, Base> {
 public:
  NODE_MACRO
  using TList::TList;
  std::unique_ptr<INode> clone() const override { return cloneList<If>(); }
};

/**
 * A when statement, composed of a List of Blocks.
 */
class When : public TList<Block, Base> {
 public:
  NODE_MACRO
  using TList::TList;
  std::unique_ptr<INode> clone() const override { return cloneList<When>(); }
};

}  // namespace cymoca::ast::statement

#endif
