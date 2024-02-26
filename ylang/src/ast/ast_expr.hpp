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

} // namespace ylang

#endif // !YL_AST_EXPR_HPP
