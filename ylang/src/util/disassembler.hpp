#ifndef YL_DISASSEMBLER_HPP
#define YL_DISASSEMBLER_HPP

#include <string>

#include "util/io.hpp"
#include "compiler/chunk.hpp"

namespace ylang {

  class Chunk;

  template <typename... Args>
  static void printinst(const std::string& fmt, Args... args) {
    printfmt(fmt, args...);
  }

  class Disassembler {
    public:
      static void Disassemble(const Chunk* chunk);

    private:
      static void DisassembleInstruction(Instruction opcode, size_t offset);

      static void SimpleInstruction(const std::string& name, size_t offset);
      static void UnaryInstruction(const std::string& name, size_t offset, Operand arg);
      static void BinaryInstruction(const std::string& name, size_t offset, Operand lhs , Operand rhs);
  };

} // namespace ylang
 
#endif // !YL_DISASSEMBLER_HPP
