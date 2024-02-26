#include "io.hpp"

#include "errors.hpp"

#include "compiler/chunk.hpp"
#include "vm/register.hpp"

namespace ylang {
  
  void print(std::string_view str) {
    fmt::print(std::string_view("{}\n"), str);
  }

  void printinstr(Instruction* inst) {
    // std::string str = fmt::format(fmt::runtime("{}"), kInstructionMap[inst->type]);
    // if (inst->lhs.has_value()) {
    //   if (*inst->lhs < kRegisterCount) {
    //     str += fmt::format(fmt::runtime(" lhs: {}"), RegisterStrings[*inst->lhs]);
    //   } else {
    //     str += fmt::format(fmt::runtime(" lhs: {}"), *inst->lhs);
    //   }
    // }
    // if (inst->rhs.has_value()) {
    //   str += fmt::format(fmt::runtime(" rhs: {}"), inst->rhs.value());
    // }
    // std::cout << str << std::endl;
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
