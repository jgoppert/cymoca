#ifndef CYMOCA_AST_STATEMENT_STATEMENT_H_
#define CYMOCA_AST_STATEMENT_STATEMENT_H_

#include "node.h"

namespace cymoca::ast::statement {

/**
 * A block of statements with a condition
 * controlling the block execution.
 */
class Block : public Base {
 protected:
  std::unique_ptr<cond::Base> condition{};
  std::vector<std::unique_ptr<Base>> statements{};

 public:
  NODE_MACRO
};

/**
 * An if statement, composed of a List of Blocks.
 */
class If : public List<Block, Base> {
 public:
  NODE_MACRO
  using List::List;
  std::unique_ptr<INode> clone() const override { return cloneList<If>(); }
};

/**
 * A when statement, composed of a List of Blocks.
 */
class When : public List<Block, Base> {
 public:
  NODE_MACRO
  using List::List;
  std::unique_ptr<INode> clone() const override { return cloneList<When>(); }
};

}  // namespace cymoca::ast::statement

#endif
