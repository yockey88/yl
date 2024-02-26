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
      void Visit(GroupingExpr& expr) override;

      void Visit(ExprStmt& stmt) override;
      void Visit(VarDeclStmt& stmt) override;
      void Visit(BlockStmt& stmt) override;

    private:
      friend class BytecodeCompiler;

      BytecodeCompiler* compiler;

      std::stack<Operand> operand_stack;
      std::stack<address_t> address_stack;

      address_t cursor{ 0 };

      void EmitOp(InstructionType type);

      void EmitLiteral(Token value);
      void EmitUnaryOp(InstructionType type , Expr* expr);
      void EmitBinaryOp(InstructionType type , Expr* lhs , Expr* rhs);
  }; 

  class BytecodeCompiler {
    public:
      BytecodeCompiler(Ast ast) 
        : ast(ast) {}
      ~BytecodeCompiler() {}

      Assembly Compile();

      RegisterType StackRegister();
      RegisterType WriteRegister();

    private:
      friend class OpEmitter;

      Ast ast;

      Chunk* current_chunk;
      Assembly assembly;

      RegisterType current_register = RegisterType::R12;

      void EmitInstruction(Instruction type);
      void EmitReturn(Operand mov_val); 
  };

} // namespace ylang

#endif // !YL_BYTECODE_COMPILER_HPP
