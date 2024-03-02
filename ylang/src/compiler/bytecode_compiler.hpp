#ifndef YL_BYTECODE_COMPILER_HPP
#define YL_BYTECODE_COMPILER_HPP

#include <stack>

#include "tree_walker.hpp"  
#include "token.hpp"
#include "ast/ast.hpp"
#include "compiler/assembly.hpp"

namespace ylang {

  class BytecodeCompiler;

  class OpEmitter : public TreeWalker {
    public:
      OpEmitter(BytecodeCompiler* compiler) 
        : compiler(compiler) {}
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

      BytecodeCompiler* compiler;

      std::stack<address_t> addr_stack;
      std::stack<Token> token_stack;
      std::stack<Value> value_stack;
      std::stack<Operand> operand_stack;

      address_t cursor{ 0 };
      address_t stack_cursor{ 0 }; 

      bool in_var_decl{ false }; 
      bool return_stmt{ false };
      const std::string* current_var_name{ nullptr };

      void EmitOp(InstructionType type);

      void EmitUnaryOp(InstructionType type , Expr* expr);
      void EmitBinaryOp(InstructionType type , Expr* lhs , Expr* rhs);
  }; 

  class BytecodeCompiler {
    public:
      BytecodeCompiler(Ast ast) 
        : ast(ast) {}
      ~BytecodeCompiler() {}

      Assembly Compile();

      Assembly& GetAssembly() { return assembly; }

      address_t stack_cursor{ 0 };

      address_t WriteVariable(Token name , Value value);
      address_t AccessVariable(Token name);
      RegisterType WriteRegister();

    private:
      friend class OpEmitter;

      Ast ast;

      Chunk* current_chunk;
      Assembly assembly;
      std::stack<Variable> scope_stack;

      uint8_t current_register = RegisterType::R12;

      void EmitInstruction(Instruction type);
  };

} // namespace ylang

#endif // !YL_BYTECODE_COMPILER_HPP
