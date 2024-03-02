#include "util/disassembler.hpp"

#include <spdlog/fmt/fmt.h>

#include "defines.hpp"

namespace ylang {

  void Disassembler::Disassemble(const Chunk* chunk) {
    for (size_t offset = 0; offset < chunk->instructions.size(); ++offset) {
      DisassembleInstruction(chunk->instructions[offset], offset);
    }
  }
      
  void Disassembler::DisassembleInstruction(Instruction opcode , size_t offset) {
    switch (opcode.type) {
      case NOP: SimpleInstruction("NOP", offset); break;

      case PUSH: UnaryInstruction("PUSH", offset, *opcode.lhs); break;
      // case POP:
      //   UnaryInstruction("POP", offset, opcode.lhs);
      // break;
      case RET: SimpleInstruction("RET", offset); break;
      // case CALL:
      //   UnaryInstruction("CALL", offset, opcode.lhs);
      // break;
      case NEG: UnaryInstruction("NEG ", offset, *opcode.lhs); break;

      // case LEA:
      //   BinaryInstruction("LEA", offset, *opcode.lhs, *opcode.rhs); 
      // break;

      case ADD: BinaryInstruction("ADD ", offset , *opcode.lhs , *opcode.rhs); break;
      case SUB: BinaryInstruction("SUB ", offset , *opcode.lhs , *opcode.rhs); break;
      case IMUL: BinaryInstruction("MUL ", offset , *opcode.lhs , *opcode.rhs); break;
      case DIV: BinaryInstruction("DIV ", offset , *opcode.lhs , *opcode.rhs); break;

      case MOV: BinaryInstruction("MOV ", offset, *opcode.lhs, *opcode.rhs); break;

      // case AND:
      //   BinaryInstruction("AND", offset, opcode.lhs, opcode.rhs);
      // break; 
      // case OR:
      //   BinaryInstruction("OR", offset, opcode.lhs, opcode.rhs);
      // break;
      // case XOR:
      //   BinaryInstruction("XOR", offset, opcode.lhs, opcode.rhs);
      // break;
      // case NOT:
      //   UnaryInstruction("NOT", offset, opcode.lhs);
      // break;

      // case JMP:
      //   UnaryInstruction("JMP", offset, opcode.lhs);
      // break;
      
      default:
        printinst("{:04x} | Unknown opcode", offset);
    }
  }
      
  void Disassembler::SimpleInstruction(const std::string& name, size_t offset) {
    printinst("{:04x} | {}", offset, name);
  }

  void Disassembler::UnaryInstruction(const std::string& name, size_t offset, Operand arg) {
    printinst("{:04x} | {}  {}", offset, name, arg);
  }

  void Disassembler::BinaryInstruction(const std::string& name, size_t offset, Operand lhs, Operand rhs) {
    std::string lhs_str = lhs.type == EMPTY ? "  " : fmtstr("{}" , lhs);
    std::string rhs_str = rhs.type == EMPTY ? "  " : fmtstr("{}" , rhs);
    printinst("{:04x} | {}  {}  {}", offset, name, lhs_str, rhs_str);
  }

} // namespace ylang
