#include "io.hpp"

#include "errors.hpp"

#include "compiler/chunk.hpp"
#include "vm/register.hpp"

namespace ylang {
  
  void print(std::string_view str) {
    fmt::print(std::string_view("{}\n"), str);
  }

  void printinstr(Instruction* inst) {
    std::string str = fmt::format(fmt::runtime("{}"), kInstructionMap[inst->type]);
    if (IsBinary(inst->type)) {
      str += fmt::format(fmt::runtime(" LHS {} RHS {}"), *inst->lhs, *inst->rhs);
    } else if (IsUnary(inst->type)) {
      str += fmt::format(fmt::runtime(" RHS {}"), *inst->rhs);
    } else {
    }
  }

  void printaddr(uint64_t addr , uint8_t* memory) {
    uint64_t val = *reinterpret_cast<uint64_t*>(&memory[addr]);
    printfmt("{:#08x} = {}", addr , val);
  }

  void printreg(Register* reg) {
    printfmt("{} = {:#08x}", RegisterStrings[reg->type], to_qword(reg));
  }

  void printstackaddr(uint64_t addr , uint8_t* memory) {
    printfmt("{:#08x} = {}", addr , to_qword(memory + addr));
  }

} // namespace ylang
