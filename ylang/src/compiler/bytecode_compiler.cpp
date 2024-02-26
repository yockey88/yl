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
  
  void OpEmitter::Visit(ExprStmt& stmt) {
    stmt.expr->Accept(*this);
  }

  void OpEmitter::Visit(VarDeclStmt& stmt) {
    Instruction inst;
    inst.type = InstructionType::MOV;
    
    inst.lhs = Operand(OperandType::REGISTER);
    inst.lhs->val = Value::CreateValue(stmt.name);

    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);
      inst.rhs = operand_stack.top();
      operand_stack.pop();
    } else {
      inst.rhs = Operand(OperandType::IMMEDIATE);
      inst.rhs->val = Value::CreateValue(stmt.type);
    }
  }

  void OpEmitter::Visit(BlockStmt& stmt) {
    /// preliminary scope setup instructions
    Instruction inst;

    for (auto& s : stmt.statements) {
      s->Accept(*this);
    }

    /// final scope teardown instructions
  }

  void OpEmitter::EmitOp(InstructionType type) {
    Instruction inst;
    inst.type = type;
    compiler->EmitInstruction(inst);
  }
    
  void OpEmitter::EmitLiteral(Token value) {
    Instruction inst;
    inst.type = InstructionType::MOV;

    inst.lhs = Operand(OperandType::REGISTER);
    size_t type_size = GetTypeSize(value.type);
    if (type_size == 0) {
      // Resolve user defined type 

    }
    inst.lhs->val = Value::CreateValue(RBP);

    inst.rhs = Operand(OperandType::IMMEDIATE);
    inst.rhs->val = Value::CreateValue(value);

    operand_stack.push(*inst.lhs);

    compiler->EmitInstruction(inst);
  }

  void OpEmitter::EmitUnaryOp(InstructionType type , Expr* expr) {
    Instruction inst;
    inst.type = type;

    expr->Accept(*this);

    inst.lhs = operand_stack.top();
    operand_stack.pop();

    operand_stack.push(*inst.lhs);

    compiler->EmitInstruction(inst);
  }

  void OpEmitter::EmitBinaryOp(InstructionType type , Expr* lhs , Expr* rhs) {
    Instruction inst;
    inst.type = type;

    rhs->Accept(*this);
    lhs->Accept(*this);

    inst.lhs = operand_stack.top();
    operand_stack.pop();

    inst.rhs = operand_stack.top();
    operand_stack.pop();

    operand_stack.push(*inst.lhs);

    compiler->EmitInstruction(inst);
  }
      

  Assembly BytecodeCompiler::Compile() {
    OpEmitter emitter(this);

    for (size_t i = 0; i < ast.Nodes().size(); ++i) {
      try {
        assembly.chunks.push_back(Chunk());
        current_chunk = &assembly.chunks.back();

        ast.Nodes()[i]->Accept(emitter);
        Operand ret = emitter.operand_stack.empty() ?
          Operand(OperandType::EMPTY) : emitter.operand_stack.top();
        if (ret.type != OperandType::EMPTY) {
          emitter.operand_stack.pop();
        }
        EmitReturn(ret);
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

    return assembly;
  }
      
  RegisterType BytecodeCompiler::WriteRegister() {
    RegisterType reg = current_register;
    current_register = static_cast<RegisterType>(static_cast<uint32_t>(current_register) + 1);
    if (current_register == RegisterType::R15) {
      current_register = RegisterType::R12;
    }
    return reg;
  }

  void BytecodeCompiler::EmitInstruction(Instruction inst) {
    current_chunk->instructions.push_back(inst);
  }
  
  void BytecodeCompiler::EmitReturn(Operand mov_val) {
    Instruction ret;
    ret.type = InstructionType::RET;

    if (mov_val.type != OperandType::EMPTY) {
      Instruction mov;
      mov.type = InstructionType::MOV;
      mov.lhs = Operand(OperandType::REGISTER);
      (*mov.lhs).val = Value::CreateValue(RegisterType::RAX);
      mov.rhs = mov_val;

      EmitInstruction(mov);
    }

    EmitInstruction(ret);
  }

} // namespace ylang
