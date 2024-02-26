#ifndef AST_STMT_HPP
#define AST_STMT_HPP

#include "ast_base.hpp"


namespace ylang {

  class ExprStmt : public Stmt {
    public:
      ExprStmt(Expr* expr) 
        : Stmt(NodeType::EXPRESSION_STMT) , expr(expr) {}
      virtual ~ExprStmt() { delete expr; }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Expr* expr = nullptr;
  };

  class VarDeclStmt : public Stmt {
    public:
      VarDeclStmt(const Token& name , Expr* initializer , 
                  const Token& type = Token(SourceLocation{} , TokenType::UNKNOWN , "")) 
        : Stmt(NodeType::VAR_DECL_STMT) , name(name) ,  type(type) , initializer(initializer){}
      virtual ~VarDeclStmt() { delete initializer; }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Token name;
      Token type;
      Expr* initializer = nullptr;
  };

  class BlockStmt : public Stmt {
    public:
      BlockStmt(const std::vector<Stmt*>& stmts) 
        : Stmt(NodeType::BLOCK_STMT) , statements(stmts) {}
      virtual ~BlockStmt() {
        for (auto stmt : statements) {
          delete stmt;
        }
      }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      std::vector<Stmt*> statements;
  };

} // namespace ylang

#endif // !AST_STMT_HPP
