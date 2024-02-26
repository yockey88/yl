#include "compiler/chunk.hpp"

#include <spdlog/fmt/fmt.h>

namespace ylang {

  bool IsUnary(InstructionType type) {
    switch (type) {
      case InstructionType::PUSH:
      case InstructionType::POP:
      case InstructionType::CALL:
      case InstructionType::NEG:
        return true;
      default:
        return false;
    }
  }

  bool IsBinary(InstructionType type) {
    switch (type) {
      case InstructionType::ADD:
      case InstructionType::SUB:
      case InstructionType::IMUL:
      case InstructionType::DIV:
      case InstructionType::MOV:
        return true;
      default: 
        return false;
    }
  }

} // namespace ylang
