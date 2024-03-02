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
    for (uint8_t reg = 0; reg < kWriteableRegisterMax; reg++) {
      delete registers[reg];
    }
  }

  void VM::Load(Assembly* assembly) {
    chunks = assembly->chunks;

#if 0
    for (auto& chunk : chunks) {
      for (auto& inst : chunk.instructions) {
        printinstr(&inst);
      }
    }
#endif
  }

  void VM::Run() {
    uint64_t chunk_index = 0;
    uint64_t instruction_index = 0;

    for (; chunk_index < chunks.size(); chunk_index++) {
      current_chunk = &chunks[chunk_index];

      for (; instruction_index < current_chunk->instructions.size(); instruction_index++) {
        try {
          instruction = &current_chunk->instructions[instruction_index];
          if (instruction->lhs->type == IMMEDIATE) {
            throw RuntimeError("Invalid immediate value in lhs of instruction");
          }

          ExecuteInstruction();
        } catch (const RuntimeError& e) {
          printfmt("Error at chunk: {} , instruction: {}" , chunk_index, instruction_index);
          printerr(ErrorType::RUNTIME , e.what());
          return;
        } catch (std::bad_variant_access& e) {
          printfmt("Invalid type access into value at chunk: {} , instruction: {}" ,chunk_index, instruction_index);
          printerr(ErrorType::RUNTIME , e.what());
          return;
        }
      }
    }

    printfmt("YVM Exit : {}" , registers[RAX]->Read());
  }

  void VM::AllocateMemory() {
    AllocateRegisters();
  }

  void VM::AllocateRegisters() {
    for (uint8_t reg = 0; reg <= kWriteableRegisterMax; reg++) {
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
    switch (type) {
      case PUSH: ExecPUSH(); break;
      case NEG: ExecNEG(); break;
      default: 
        throw RuntimeError("Unknown unary instruction type");
    }
  }

  void VM::ExecBinaryOp(InstructionType type) {
    switch (type) {
      case MOV: ExecMOV(); break;
      case ADD: ExecADD(); break;
      case SUB: ExecSUB(); break;
      case IMUL: ExecIMUL(); break;
      case DIV: ExecDIV(); break;
      default: 
        throw RuntimeError("Unknown binary instruction type");
    }
  }

  void VM::ExecPUSH() {
  }

  void VM::ExecPOP() {
    assert(false && "POP is not implemented");
  }

  void VM::ExecRET() {
    printfmt("RET {}" , registers[RAX]->Read());
    // for now leave rax as the return value
  }

  void VM::ExecCALL() {
    assert(false && "CALL is not implemented");
  }

  void VM::ExecNEG() {
  }
  
  void VM::ExecLEA() {}

  void VM::ExecADD() {
  }

  void VM::ExecSUB() {
  }

  void VM::ExecIMUL() {
  }

  void VM::ExecDIV() {
  }

  void VM::ExecMOV() {
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
      RegisterType reg = op.val.As<RegisterType>();
      if (reg == RBP || reg == RSP) {
        throw RuntimeError("Stack unimplemented");
      }
      registers[reg]->Write(val);
    } else if (op.type == OperandType::DIRECT) {
      // Write(op.val.As<address_t>(), val);
    } else {
      throw RuntimeError("Unknown operand type in Write");
    }
  }

  void VM::DumpMemory() {
  }

} // namespace ylang
