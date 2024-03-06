#ifndef YL_RESOLVER_HPP
#define YL_RESOLVER_HPP

#include <stack>
#include <unordered_map>

#include "errors.hpp"
#include "tree_walker.hpp"
#include "ast/ast.hpp"

namespace ylang {
  
  class DeclarationResolver : public TreeWalker {
    public:
      DeclarationResolver(const std::vector<Stmt*>& stmts) 
        : stmts(stmts) {}
      virtual ~DeclarationResolver() {}

      void Resolve();

      // Expressions
      virtual void Visit(LiteralExpr& expr) override final;
      virtual void Visit(UnaryExpr& expr) override final;
      virtual void Visit(BinaryExpr& expr) override final;
      virtual void Visit(CallExpr& expr) override final;
      virtual void Visit(GroupingExpr& expr) override final;
      virtual void Visit(VarExpr& expr) override final;
      virtual void Visit(AssignExpr& expr) override final;
      virtual void Visit(ArrayExpr& expr) override final;
      virtual void Visit(ArrayAccessExpr& expr) override final;
      virtual void Visit(ObjAccessExpr& expr) override final;

      // Statements
      virtual void Visit(ExprStmt& stmt) override final;
      virtual void Visit(VarDeclStmt& stmt) override final;
      virtual void Visit(ArrayDeclStmt& stmt) override final;
      virtual void Visit(BlockStmt& stmt) override final;
      virtual void Visit(PrintStmt& stmt) override final;
      virtual void Visit(IfStmt& stmt) override final;
      virtual void Visit(WhileStmt& stmt) override final;
      virtual void Visit(ReturnStmt& stmt) override final;
      virtual void Visit(FunctionStmt& stmt) override final;
      virtual void Visit(StructStmt& stmt) override final;

    private:
      const std::vector<Stmt*>& stmts;

      std::stack<Token> token_stack;

      constexpr static uint32_t kMaxScopes = 100;
      std::array<std::unordered_map<std::string , bool> , kMaxScopes> scopes;

      int32_t current_scope = 0;

      void BeginScope();
      void EndScope();

      void Resolve(std::vector<Stmt*>& stmts);
      void Resolve(Stmt* stmt);
      void Resolve(Expr* expr);

      void ResolveLocal(Expr& expr , const Token& name);
      void ResolveFunction(FunctionStmt& stmt);

      void Declare(const Token& name);
      void Define(const Token& name);

      void DumpScopes();

      StaticAnalysisError Error(const Token& token , const std::string& message , ErrorType type = ErrorType::STATIC_ANALYSIS);
  };

  class Resolver {
    public:
      Resolver(Ast& statements)
        : statements(statements) {}
      ~Resolver() {}

      bool Resolve();

    private:
      Ast& statements;
  };


} // namespace ylang

#endif // !YL_RESOLVER_HPP
