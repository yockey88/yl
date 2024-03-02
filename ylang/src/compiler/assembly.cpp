#include "compiler/assembly.hpp"

#include "defines.hpp"
#include "util/io.hpp"
#include "util/disassembler.hpp"

namespace ylang {

  address_t Assembly::Write(const std::string& name , Value var) {
    return address_t{0};
  }
      
  address_t Assembly::ReadAddress(const std::string& name) {
    return address_t{0};
  }

  Variable Assembly::Read(address_t addr) {
    return nil_var;
  }
      
  Variable Assembly::Read(const std::string& name) {
    return nil_var;
  }

  void Assembly::Dump() {
    print("=== Assembly ===");
    print("  - Memory -");
    DumpMemory();
    print("  - Chunks -");
    for (uint32_t i = 0; i < chunks.size(); ++i) {
      printchunk(&chunks[i], fmtstr("Chunk {}", i));   
    }
  }

  void Assembly::DumpMemory() {
    for (uint64_t i = 0; i < 10; i++) {
      printaddr(address_t{i});
    }
  }
      
  void Assembly::printchunk(const Chunk* chunk, const std::string& name) {
    printfmt("  -- {} --", name);
    Disassembler::Disassemble(chunk);
    printfmt("  -- END {} --" , name);
  }

  void Assembly::printaddr(address_t addr) {
  }

} // namespace ylang

