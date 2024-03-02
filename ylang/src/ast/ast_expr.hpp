#ifndef YL_AST_EXPR_HPP
#define YL_AST_EXPR_HPP

#include "token.hpp"
#include "ast/ast_base.hpp"

namespace ylang {

  class LiteralExpr : public Expr {
    public:
      LiteralExpr(Token value) 
        : Expr(NodeType::LITERAL_EXPR) , value(value) {}
      ~LiteralExpr() {}

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Token value;
  };

  class UnaryExpr : public Expr {
    public:
      UnaryExpr(Token op, Expr* right) 
        : Expr(NodeType::UNARY_EXPR) , op(op), right(right) {}
      ~UnaryExpr();

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Token op;
      Expr* right;
  };

  class BinaryExpr : public Expr {
    public:
      BinaryExpr(Expr* left, Token op, Expr* right) 
        : Expr(NodeType::BINARY_EXPR) , left(left), op(op), right(right) {}
      ~BinaryExpr(); 

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Expr* left;
      Token op;
      Expr* right;
  };

  class CallExpr : public Expr {
    public:
      CallExpr(Expr* callee, std::vector<Expr*> args) 
        : Expr(NodeType::CALL_EXPR) , callee(callee), args(args) {}
      ~CallExpr();

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Expr* callee;
      std::vector<Expr*> args;
  };

  class GroupingExpr : public Expr {
    public:
      GroupingExpr(Expr* expr) 
        : Expr(NodeType::GROUPING_EXPR) , expr(expr) {}
      ~GroupingExpr();

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Expr* expr;
  };

  class VarExpr : public Expr {
    public:
      VarExpr(Token name) 
        : Expr(NodeType::VAR_EXPR) , name(name) {}
      ~VarExpr();

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Token name;
  };

  class AssignExpr : public Expr {
    public:
      AssignExpr(Token name, Expr* value) 
        : Expr(NodeType::ASSIGN_EXPR) , name(name), value(value) {}
      ~AssignExpr();

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Token name;
      Expr* value;
  };

  class ArrayExpr : public Expr {
    public:
      ArrayExpr(std::vector<Expr*> elements) 
        : Expr(NodeType::ARRAY_EXPR) , elements(elements) {}
      ~ArrayExpr();

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      std::vector<Expr*> elements;
  };

  class ArrayAccessExpr : public Expr {
    public:
      ArrayAccessExpr(Expr* array, Expr* index) 
        : Expr(NodeType::ARRAY_ACCESS_EXPR) , array(array), index(index) {}
      ~ArrayAccessExpr();

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Expr* array;
      Expr* index;
  };

  class ObjAccessExpr : public Expr {
    public:
      ObjAccessExpr(Expr* obj, Token member , Expr* assignment = nullptr) 
        : Expr(NodeType::OBJ_ACCESS_EXPR) , obj(obj), member(member) , assignment(assignment) {}
      ~ObjAccessExpr() { delete obj; }

      std::string ToString() const override;
      void Accept(TreeWalker& walker) override;
      std::vector<Instruction> Emit() override;

      Expr* obj;
      Token member;
      Expr* assignment;
  };

} // namespace ylang

#endif // !YL_AST_EXPR_HPP
