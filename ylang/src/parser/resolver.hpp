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
      DeclarationResolver(const Ast& stmts) 
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
      struct Variable {
        std::string name;
        Value::Type type;
        bool defined = false;
      };

      struct Function {
        std::string name;
        Value::Type type;
        bool defined = false;
      };

      struct Scope {
        std::unordered_map<uint64_t , Variable> variables;
        std::map<address_t , Function> declared_funcs;

        address_t current_func = 0x0;

        operator std::unordered_map<uint64_t , Variable>&() { return variables; }
        Variable& operator[](uint64_t i) { return variables[i]; }

        auto begin() { return variables.begin(); }
        auto end() { return variables.end(); }
        auto find(uint64_t i) { return variables.find(i); }
        auto clear() { variables.clear(); }
      };

      const Ast& stmts;

      bool need_identifier = false;

      std::stack<Token> token_stack;
      std::stack<Value::Type> type_stack;

      constexpr static uint32_t kMaxScopeDepth = 100;
      std::array<Scope , kMaxScopeDepth> scopes;
      // std::array<std::unordered_map<std::string , bool> , kMaxScopeDepth> scopes;

      uint32_t current_scope = 0;

      Variable CreateVar(const Token& name , const Token& type);
      Value::Type GetDeclaredType(const Token& name);
      void SetType(const Token& name , Value::Type type);

      void BeginScope();
      void EndScope();

      void Resolve(std::vector<Stmt*>& stmts);
      void Resolve(Stmt* stmt);
      void Resolve(Expr* expr);

      void ResolveLocal(Expr& expr , const Token& name);
      void ResolveFunction(FunctionStmt& stmt);
      void ResolveType(const Token& name , const Token& type);

      void Declare(const Token& name , const Token& type);
      void Define(const Token& name , const Token& type);
      void DeclareFunction(const std::string& name , Value::Type type);
      void DefineFunction(const std::string& name , Value::Type type);
      bool IsFunction(const std::string& name);
      Value::Type GetFunctionType(const std::string& name);

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
