#ifndef YL_TREE_WALKER_HPP
#define YL_TREE_WALKER_HPP

namespace ylang {

  class Node;
  class Expr;
  class Stmt;

  class LiteralExpr;
  class UnaryExpr;
  class BinaryExpr;
  class GroupingExpr;

  class ExprStmt;
  class VarDeclStmt;
  class BlockStmt;

  class TreeWalker {
    public:
      virtual ~TreeWalker() = default;

      // Expressions
      virtual void Visit(LiteralExpr& expr) = 0;
      virtual void Visit(UnaryExpr& expr) = 0;
      virtual void Visit(BinaryExpr& expr) = 0;
      virtual void Visit(GroupingExpr& stmt) = 0;

      // Statements
      virtual void Visit(ExprStmt& stmt) = 0;
      virtual void Visit(VarDeclStmt& stmt) = 0;
      virtual void Visit(BlockStmt& stmt) = 0;
  };

} // namespace ylang

#endif // !YL_TREE_WALKER_HPP
