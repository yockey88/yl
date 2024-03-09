#ifndef YL_SYM_EXTRACTOR_HPP
#define YL_SYM_EXTRACTOR_HPP

#include <stack>
#include <string>

#include "tree_walker.hpp"
#include "ast/ast.hpp"
#include "env/symbol_table.hpp"

namespace ylang {

  class SymbolExtractor : public TreeWalker {
    public:
      SymbolExtractor(SymbolTable& table) 
        : table(table) {}
      virtual ~SymbolExtractor() {}

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
      SymbolTable& table;

      std::stack<Value> value_stack;
      std::stack<std::string> identifier_stack;
      std::stack<std::string> scope_name_stack;

      bool need_identifier = false;
      bool struct_decl = false;
      bool var_decl = false;
      std::optional<std::string> current_func;

      std::string BuildScopeName(const std::string& name);
  }; 

  class TableBuilder {
    public:
      TableBuilder(Ast& stmts)
        : stmts(stmts) {}

      SymbolTable Build();

    private:
      Ast& stmts;

      SymbolTable table = SymbolTable(stmts.name);
  };

} // namespace ylang

#endif // !YL_SYM_EXTRACTOR_HPP
