#include "compiler/bytecode_compiler.hpp"

#include <cassert>

#include "value.hpp"
#include "errors.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {

  void OpEmitter::Visit(LiteralExpr& expr) {
    EmitLiteral(expr.value);
  }

  void OpEmitter::Visit(UnaryExpr& expr) {
    switch (expr.op.type) {
      case TokenType::PLUS:
        expr.right->Accept(*this);
      break;
      case TokenType::MINUS:
        EmitUnaryOp(InstructionType::NEG , expr.right);
      break;
      default:
        assert(false);
    }
  }

  void OpEmitter::Visit(BinaryExpr& expr) {
    switch (expr.op.type) {
      case TokenType::PLUS:
        EmitBinaryOp(InstructionType::ADD , expr.left , expr.right);
      break;
      case TokenType::MINUS:
        EmitBinaryOp(InstructionType::SUB , expr.left , expr.right);
      break;
      case TokenType::STAR:
        EmitBinaryOp(InstructionType::IMUL , expr.left , expr.right);
      break; 
      case TokenType::F_SLASH:
        EmitBinaryOp(InstructionType::DIV , expr.left , expr.right);
      break;
      default:
        assert(false);
    }
  }

  void OpEmitter::Visit(GroupingExpr& expr) {
    expr.expr->Accept(*this);
  }

  void OpEmitter::Visit(VarExpr& expr) {
    address_t addr = compiler->WriteVariable(expr.name);
  }
  
  void OpEmitter::Visit(ExprStmt& stmt) {
    stmt.expr->Accept(*this);
  }

  void OpEmitter::Visit(VarDeclStmt& stmt) {
    Instruction inst;
    inst.type = InstructionType::PUSH;

    if (stmt.initializer == nullptr) {
      inst.lhs = Operand(OperandType::IMMEDIATE);
      inst.lhs->val = Value::CreateValue(stmt.type);

      operand_stack.push(*inst.lhs);

      compiler->EmitInstruction(inst);
    } else {
      stmt.initializer->Accept(*this);

      if (token_stack.size() > 0) {
        inst.lhs = Operand(OperandType::DIRECT);
        inst.lhs->val = Value::CreateValue(token_stack.top());
        token_stack.pop();
      } else {
        inst.lhs = operand_stack.top();
        operand_stack.pop();
      }
    }
  }

  void OpEmitter::Visit(BlockStmt& stmt) {
    Instruction inst;

    for (auto& s : stmt.statements) {
      s->Accept(*this);
    }
  }

  void OpEmitter::EmitOp(InstructionType type) {
    Instruction inst;
    inst.type = type;
    compiler->EmitInstruction(inst);
  }
    
  void OpEmitter::EmitLiteral(Token value) {
    Instruction inst;
    inst.type = InstructionType::PUSH;

    inst.lhs = Operand(OperandType::IMMEDIATE);
    inst.lhs->val = Value::CreateValue(value);

    operand_stack.push(*inst.lhs);

    compiler->EmitInstruction(inst);
  }

  void OpEmitter::EmitUnaryOp(InstructionType type , Expr* expr) {
    Instruction inst;
    inst.type = type;

    expr->Accept(*this);

    Operand op = operand_stack.top();
    operand_stack.pop();

    compiler->EmitInstruction(inst);
  }

  void OpEmitter::EmitBinaryOp(InstructionType type , Expr* lhs , Expr* rhs) {
    Instruction inst;
    inst.type = type;

    rhs->Accept(*this);
    lhs->Accept(*this);

    Operand lhs_op = operand_stack.top();
    operand_stack.pop();

    Operand rhs_op = operand_stack.top();
    operand_stack.pop();

    compiler->EmitInstruction(inst);
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

    Instruction ret;
    ret.type = InstructionType::RET;
    EmitInstruction(ret);

    return assembly;
  }

  address_t BytecodeCompiler::WriteVariable(Token name) {
    return assembly.Write(name);
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
