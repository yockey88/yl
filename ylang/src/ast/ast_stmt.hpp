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

  class ArrayDeclStmt : public Stmt {
    public:
      ArrayDeclStmt(const Token& name , Expr* initializer , Token size , 
                    const Token& type = Token(SourceLocation{} , TokenType::UNKNOWN , "")) 
        : Stmt(NodeType::ARRAY_DECL_STMT) , name(name) , type(type)  , 
          size(size) , initializer(initializer) {}
      virtual ~ArrayDeclStmt() { delete initializer; }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Token name;
      Token type;
      Token size;
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

  class PrintStmt : public Stmt {
    public:
      PrintStmt(Expr* expr) 
        : Stmt(NodeType::PRINT_STMT) , expr(expr) {}
      virtual ~PrintStmt() { delete expr; }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Expr* expr;
  };

  class IfStmt : public Stmt {
    public:
      IfStmt(Expr* condition , Stmt* thenBranch , Stmt* elseBranch = nullptr) 
        : Stmt(NodeType::IF_STMT) , condition(condition) , then_branch(then_branch) , else_branch(else_branch) {}
      virtual ~IfStmt() {
        delete condition;
        delete then_branch;
        delete else_branch;
      }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Expr* condition;
      Stmt* then_branch;
      Stmt* else_branch;
  };

  class WhileStmt : public Stmt {
    public:
      WhileStmt(Expr* condition , Stmt* body) 
        : Stmt(NodeType::WHILE_STMT) , condition(condition) , body(body) {}
      virtual ~WhileStmt() {
        delete condition;
        delete body;
      }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Expr* condition;
      Stmt* body;
  };

  class ReturnStmt : public Stmt {
    public:
      ReturnStmt(Expr* expr = nullptr) 
        : Stmt(NodeType::RETURN_STMT) , expr(expr) {}
      virtual ~ReturnStmt() { delete expr; }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Expr* expr = nullptr;
  };

  class FunctionStmt : public Stmt {
    public:
      FunctionStmt(const Token& name , const std::vector<Token>& params , const Token& type ,
                   Stmt* body = nullptr) 
        : Stmt(NodeType::FUNCTION_STMT) , name(name) , params(params) , body(body) , type(type) {}
      virtual ~FunctionStmt() {
        delete body;
      }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Token name;
      std::vector<Token> params;
      Stmt* body;
      Token type;
  };

  class StructStmt : public Stmt {
    public:
      StructStmt(const Token& name , const std::vector<Stmt*>& fields) 
        : Stmt(NodeType::STRUCT_STMT) , name(name) , fields(fields) {}
      virtual ~StructStmt() {
        for (auto field : fields) {
          delete field;
        }
      }

      virtual std::string ToString() const override;
      virtual void Accept(TreeWalker& walker) override;
      virtual std::vector<Instruction> Emit() override;

      Token name;
      std::vector<Stmt*> fields;
  };

} // namespace ylang

#endif // !AST_STMT_HPP
