#include "vm/vm.hpp"

#include <cassert>

#include "defines.hpp"
#include "errors.hpp"
#include "util/io.hpp"
#include "compiler/assembly.hpp"

namespace ylang {

  VM::VM() {
    AllocateMemory();
  }

  VM::~VM() {
    for (uint8_t reg = 0; reg < kRegisterCount; reg++) {
      delete registers[reg];
    }
  }

  void VM::Load(Assembly* assembly) {
    chunks = assembly->chunks;

    const uint8_t* const mem = assembly->ReadMemory();
    for (uint64_t i = 0; i < kHeapMemorySize; i++) {
      memory[i] = mem[i];
    }
  }

  void VM::Run() {
    uint64_t chunk_index = 0;
    uint64_t instruction_index = 0;

    for (; chunk_index < chunks.size(); chunk_index++) {
      current_chunk = &chunks[chunk_index];

      for (; instruction_index < current_chunk->instructions.size(); instruction_index++) {
        try {
          this->instruction = &current_chunk->instructions[instruction_index];

          ExecuteInstruction();
        } catch (const RuntimeError& e) {
          printfmt("Error at chunk: {} , instruction: {}\n" , chunk_index, instruction_index);
          printerr(ErrorType::RUNTIME , e.what());
          return;
        } catch (std::bad_variant_access& e) {
          printfmt("Invalid type access into value at chunk: {} , instruction: {}" ,chunk_index, instruction_index);
          printerr(ErrorType::RUNTIME , e.what());
          return;
        }
      }
    }
  }

  void VM::AllocateMemory() {
    AllocateRegisters();
  }

  void VM::AllocateRegisters() {
    for (uint8_t reg = 0; reg < kRegisterCount; reg++) {
      registers[reg] = new Register();
      registers[reg]->type = static_cast<RegisterType>(reg);
    }
  }

  void VM::ExecuteInstruction() {
#if 0
    printfmt("Executing instruction: {}\n", kInstructionMap[instruction->type]);
#endif

    auto inst_type = instruction->type;

    if (IsUnary(inst_type)) {
      ExecUnaryOp(inst_type);
      return;
    }

    if (IsBinary(inst_type)) {
      ExecBinaryOp(inst_type);
      return;
    }

    switch (inst_type) {
      case RET: ExecRET(); break;
      default: 
        throw RuntimeError("Unknown instruction type");
    }
  }
  
  void VM::ExecUnaryOp(InstructionType type) {
    if (instruction->lhs->type == OperandType::IMMEDIATE) {
      throw RuntimeError("Unary operation on immediate not supported");
    } else {
      if (instruction->lhs->type == OperandType::REGISTER) {
        RegisterType reg = instruction->lhs->val.As<RegisterType>();
        stack.push(registers[reg]->Read());
      } else if (instruction->lhs->type == OperandType::DIRECT) {
        throw RuntimeError("Unary operation on direct memory not supported");
      } else {
        throw RuntimeError("Unary operation on memory not supported");
      }
    }

    switch (type) {
      case NEG: ExecNEG(); break;
      default: 
        throw RuntimeError("Unknown unary instruction type");
    }
  }

  void VM::ExecBinaryOp(InstructionType type) {
    if (type == MOV) {
      if (instruction->lhs->type == OperandType::IMMEDIATE) {
        throw RuntimeError("MOV into immediate operand not supported");
      } else {
        if (instruction->rhs->type == OperandType::IMMEDIATE) {
          stack.push(instruction->rhs->val);
        } else if (instruction->rhs->type == OperandType::REGISTER) {
          RegisterType reg = instruction->rhs->val.As<RegisterType>();
          stack.push(registers[reg]->Read());
        } else {
          throw RuntimeError("MOV into memory not supported");
        }
      }
      ExecMOV(); 
      return;
    }

    if (instruction->lhs->type == OperandType::IMMEDIATE) {
      stack.push(instruction->lhs->val);
    } else if (instruction->lhs->type == OperandType::REGISTER) {
      RegisterType reg = instruction->lhs->val.As<RegisterType>();
      stack.push(registers[reg]->Read());
    } else {
      throw RuntimeError("ADD into memory not supported");
    }

    if (instruction->rhs->type == OperandType::IMMEDIATE) {
      stack.push(instruction->rhs->val);
    } else if (instruction->rhs->type == OperandType::REGISTER) {
      RegisterType reg = instruction->rhs->val.As<RegisterType>();
      stack.push(registers[reg]->Read());
    } else {
      throw RuntimeError("ADD into memory not supported");
    }

    switch (type) {
      case ADD: ExecADD(); break;
      case SUB: ExecSUB(); break;
      case IMUL: ExecIMUL(); break;
      case DIV: ExecDIV(); break;
      default: 
        throw RuntimeError("Unknown binary instruction type");
    }
  }

  void VM::ExecPUSH() {
    assert(false && "PUSH is not implemented");
  }

  void VM::ExecPOP() {
    assert(false && "POP is not implemented");
  }

  void VM::ExecRET() {
    Value val = registers[RAX]->Read();
    printfmt("RET : {}\n" , val);
  }

  void VM::ExecCALL() {
    assert(false && "CALL is not implemented");
  }

  void VM::ExecNEG() {
    Value val = stack.top();
    stack.pop();

    val.Negate();

    Write(*instruction->lhs, val);
  }
  
  void VM::ExecLEA() {}

  void VM::ExecADD() {
    Value rval = stack.top();
    stack.pop();

    Value lval = stack.top();
    stack.pop();

    Write(*instruction->lhs, lval + rval);
  }

  void VM::ExecSUB() {
    Value rval = stack.top();
    stack.pop();

    Value lval = stack.top();
    stack.pop();

    Write(*instruction->lhs, lval - rval);
  }

  void VM::ExecIMUL() {
    Value rval = stack.top();
    stack.pop();

    Value lval = stack.top();
    stack.pop();

    Write(*instruction->lhs, lval * rval);
  }

  void VM::ExecDIV() {
    Value rval = stack.top();
    stack.pop();

    Value lval = stack.top();
    stack.pop();

    Write(*instruction->lhs, lval / rval);
  }

  void VM::ExecMOV() {
    if (instruction->lhs->type == OperandType::IMMEDIATE) {
      throw RuntimeError("MOV into immediate not supported");
    }

    Value rval = stack.top();
    stack.pop();

    Write(*instruction->lhs, rval);
  }

  void VM::ExecAND() {
    assert(false && "AND is not implemented");
  }

  void VM::ExecOR() {
    assert(false && "OR is not implemented");
  }
  
  void VM::ExecXOR() {
    assert(false && "XOR is not implemented");
  }

  void VM::ExecNOT() {
    assert(false && "NOT is not implemented");
  }

  void VM::ExecJMP() {
    assert(false && "JMP is not implemented");
  }

  Value VM::Read(Operand& op) {
    if (op.type == OperandType::REGISTER) {
      return registers[op.val.As<RegisterType>()]->Read(); 
    } else if (op.type == OperandType::DIRECT) {
      return Value{}; // Read(op.val.As<address_t>());
    } else if (op.type == OperandType::IMMEDIATE) {
      return op.val;
    } else {
      throw RuntimeError("Unknown operand type in Read");
    }
  }
      
  void VM::Write(Operand& op, const Value& val) {
    if (op.type == OperandType::REGISTER) {
      registers[op.val.As<RegisterType>()]->Write(val);
    } else if (op.type == OperandType::DIRECT) {
      // Write(op.val.As<address_t>(), val);
    } else {
      throw RuntimeError("Unknown operand type in Write");
    }
  }

  void VM::DumpMemory() {
    // std::cout << "=== Memory ===" << std::endl;
    // for (uint64_t i = 0; i < kMemorySize; i += sizeof(uint64_t)) {
    //   printaddr(i);
    // }
  }

  void VM::DumpRegisters() {
    // std::cout << "=== Registers ===" << std::endl;
    // for (uint8_t reg = 0; reg < kRegisterCount; reg++) {
    //   fmt::print(fmt::runtime("{} = {}\n") , RegisterStrings[reg] , registers[reg]->Read());
    // }
  }

} // namespace ylang
