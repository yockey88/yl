#ifndef YL_BYTECODE_COMPILER_HPP
#define YL_BYTECODE_COMPILER_HPP

#include "tree_walker.hpp"  
#include "compiler/assembly.hpp"

namespace ylang {

  class BytecodeCompiler;

  class OpEmitter : public TreeWalker {
    public:
      OpEmitter(const SymbolTable* table) 
        : table(table) {}
      ~OpEmitter() {}

      void Visit(LiteralExpr& expr) override;
      void Visit(UnaryExpr& expr) override;
      void Visit(BinaryExpr& expr) override;
      void Visit(CallExpr& expr) override;
      void Visit(GroupingExpr& expr) override;
      void Visit(VarExpr& expr) override;
      void Visit(AssignExpr& expr) override;
      void Visit(ArrayExpr& expr) override;
      void Visit(ArrayAccessExpr& expr) override;
      void Visit(ObjAccessExpr& expr) override;

      void Visit(ExprStmt& stmt) override;
      void Visit(VarDeclStmt& stmt) override;
      void Visit(ArrayDeclStmt& stmt) override;
      void Visit(BlockStmt& stmt) override;
      void Visit(PrintStmt& stmt) override;
      void Visit(IfStmt& stmt) override;
      void Visit(WhileStmt& stmt) override;
      void Visit(ReturnStmt& stmt) override;
      void Visit(FunctionStmt& stmt) override;
      void Visit(StructStmt& stmt) override;

    private:
      friend class BytecodeCompiler;

      const SymbolTable* table;
  }; 

  class BytecodeCompiler {
    public:
      BytecodeCompiler(const SymbolTable& table) 
        : table(table) {}
      ~BytecodeCompiler() {}

      Assembly Compile();

    private:
      friend class OpEmitter;

      const SymbolTable& table;
  };

} // namespace ylang

#endif // !YL_BYTECODE_COMPILER_HPP
