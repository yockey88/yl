#ifndef YL_TREE_WALKER_HPP
#define YL_TREE_WALKER_HPP

namespace ylang {

  class Node;
  class Expr;
  class Stmt;

  class LiteralExpr;
  class UnaryExpr;
  class BinaryExpr;
  class CallExpr;
  class GroupingExpr;
  class VarExpr;
  class AssignExpr;
  class ArrayExpr;
  class ArrayAccessExpr;
  class ObjAccessExpr;

  class ExprStmt;
  class VarDeclStmt;
  class ArrayDeclStmt;
  class BlockStmt;
  class PrintStmt;
  class IfStmt;
  class WhileStmt;
  class ReturnStmt;
  class FunctionStmt;
  class StructStmt;

  class TreeWalker {
    public:
      virtual ~TreeWalker() = default;

      // Expressions
      virtual void Visit(LiteralExpr& expr) = 0;
      virtual void Visit(UnaryExpr& expr) = 0;
      virtual void Visit(BinaryExpr& expr) = 0;
      virtual void Visit(CallExpr& expr) = 0;
      virtual void Visit(GroupingExpr& stmt) = 0;
      virtual void Visit(VarExpr& stmt) = 0;
      virtual void Visit(AssignExpr& stmt) = 0;
      virtual void Visit(ArrayExpr& stmt) = 0;
      virtual void Visit(ArrayAccessExpr& stmt) = 0;
      virtual void Visit(ObjAccessExpr& stmt) = 0;

      // Statements
      virtual void Visit(ExprStmt& stmt) = 0;
      virtual void Visit(VarDeclStmt& stmt) = 0;
      virtual void Visit(ArrayDeclStmt& stmt) = 0;
      virtual void Visit(BlockStmt& stmt) = 0;
      virtual void Visit(PrintStmt& stmt) = 0;
      virtual void Visit(IfStmt& stmt) = 0;
      virtual void Visit(WhileStmt& stmt) = 0;
      virtual void Visit(ReturnStmt& stmt) = 0;
      virtual void Visit(FunctionStmt& stmt) = 0;
      virtual void Visit(StructStmt& stmt) = 0;
  };

} // namespace ylang

#endif // !YL_TREE_WALKER_HPP
