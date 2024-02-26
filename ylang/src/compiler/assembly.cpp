#include "compiler/assembly.hpp"

#include "value.hpp"
#include "defines.hpp"
#include "util/io.hpp"
#include "util/disassembler.hpp"

namespace ylang {

  address_t Assembly::Write(Token token) {
    return address_t{0};
  }
      
  Value Assembly::Read(address_t offset) {
    // if (offset > kMemorySize) {
    //   printf("Memory overflow\n");
    //   return 0;
    // }

    // uint64_t val = 0;
    // uint8_t* data = reinterpret_cast<uint8_t*>(&val);
    // for (uint64_t i = 0; i < sizeof(uint64_t); ++i) {
    //   data[i] = memory[offset + i];
    // }

    return Value{};
  }

  void Assembly::Dump() {
    for (uint32_t i = 0; i < chunks.size(); ++i) {
      printchunk(&chunks[i], fmt::format(fmt::runtime("Chunk {}"), i));   
    }
  }

  void Assembly::DumpMemory() {
    // for (uint64_t i = 0; i < kMemorySize; i += sizeof(uint64_t)) {
    //   printaddr(i);
    // }
    for (uint64_t i = 0; i < 10 * sizeof(uint64_t); i += sizeof(uint64_t)) {
      printaddr(address_t{i});
    }
  }
      
  void Assembly::printchunk(const Chunk* chunk, const std::string& name) {
    printfmt("=== {} ===", name);
    Disassembler::Disassemble(chunk);
    printfmt("=== END {} ===" , name);
  }

  void Assembly::printaddr(address_t addr) {
  }

} // namespace ylang

