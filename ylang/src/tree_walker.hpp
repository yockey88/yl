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
      virtual void Visit(LiteralExpr& expr) {}
      virtual void Visit(UnaryExpr& expr) {}
      virtual void Visit(BinaryExpr& expr) {}
      virtual void Visit(CallExpr& expr) {}
      virtual void Visit(GroupingExpr& expr) {}
      virtual void Visit(VarExpr& expr) {}
      virtual void Visit(AssignExpr& expr) {}
      virtual void Visit(ArrayExpr& expr) {}
      virtual void Visit(ArrayAccessExpr& expr) {}
      virtual void Visit(ObjAccessExpr& expr) {}

      // Statements
      virtual void Visit(ExprStmt& stmt) {}
      virtual void Visit(VarDeclStmt& stmt) {}
      virtual void Visit(ArrayDeclStmt& stmt) {}
      virtual void Visit(BlockStmt& stmt) {}
      virtual void Visit(PrintStmt& stmt) {}
      virtual void Visit(IfStmt& stmt) {}
      virtual void Visit(WhileStmt& stmt) {}
      virtual void Visit(ReturnStmt& stmt) {}
      virtual void Visit(FunctionStmt& stmt) {}
      virtual void Visit(StructStmt& stmt) {}
  };

} // namespace ylang

#endif // !YL_TREE_WALKER_HPP
