#include "compiler/bytecode_compiler.hpp"

#include "value.hpp"
#include "errors.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {

  void OpEmitter::Visit(LiteralExpr& expr) {
  }

  void OpEmitter::Visit(UnaryExpr& expr) {
  }

  void OpEmitter::Visit(BinaryExpr& expr) {
  }

  void OpEmitter::Visit(CallExpr& expr) {
  }

  void OpEmitter::Visit(GroupingExpr& expr) {
  }

  void OpEmitter::Visit(VarExpr& expr) {
  }

  void OpEmitter::Visit(AssignExpr& expr) {
  }

  void OpEmitter::Visit(ArrayExpr& expr) {
  }

  void OpEmitter::Visit(ArrayAccessExpr& expr) {
  }

  void OpEmitter::Visit(ObjAccessExpr& expr) {
  }
  
  void OpEmitter::Visit(ExprStmt& stmt) {
  }

  void OpEmitter::Visit(VarDeclStmt& stmt) {
  }

  void OpEmitter::Visit(ArrayDeclStmt& stmt) {
  }

  void OpEmitter::Visit(BlockStmt& stmt) {
  }

  void OpEmitter::Visit(PrintStmt& stmt) {
  }

  void OpEmitter::Visit(IfStmt& stmt) {
  }

  void OpEmitter::Visit(WhileStmt& stmt) {
  }

  void OpEmitter::Visit(ReturnStmt& stmt) {
  }

  void OpEmitter::Visit(FunctionStmt& stmt) {
  }

  void OpEmitter::Visit(StructStmt& stmt) {
  }

  void OpEmitter::EmitOp(InstructionType type) {
  }

  void OpEmitter::EmitUnaryOp(InstructionType type , Expr* expr) {
  }

  void OpEmitter::EmitBinaryOp(InstructionType type , Expr* lhs , Expr* rhs) {
  }
      

  Assembly BytecodeCompiler::Compile() {
    OpEmitter emitter(this);

    assembly.chunks.push_back(Chunk());
    current_chunk = &assembly.chunks.back();

    for (size_t i = 0; i < ast.Nodes().size(); ++i) {
      try {

        ast.Nodes()[i]->Accept(emitter);

      } catch (const CompilerError& e) {
        printerr(ErrorType::COMPILER , "Compiler Failure : " + std::string{ e.what() });
        break;
      } catch (const std::bad_variant_access& e) {
        printerr(ErrorType::COMPILER , "Type Error : " + std::string{ e.what() });
        break;
      } catch (const std::exception& e) {
        printerr(ErrorType::COMPILER , std::string{ e.what() });
        break;
      }
    }

    return std::move(assembly);
  }

  address_t BytecodeCompiler::WriteVariable(Token name , Value value) {
    return address_t{ 0 };
  }

  address_t BytecodeCompiler::AccessVariable(Token name) {
    return address_t{ 0 };
  }

  RegisterType BytecodeCompiler::WriteRegister() {
    RegisterType reg{ current_register };
    current_register++;
    if (RegisterType{ current_register } == RegisterType::RBP) {
      current_register = RegisterType::R12;
    }

    return reg;
  }

  void BytecodeCompiler::EmitInstruction(Instruction inst) {
    current_chunk->instructions.push_back(inst);
  }

} // namespace ylang
