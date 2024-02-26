#include "compiler/assembly.hpp"

#include "value.hpp"
#include "defines.hpp"
#include "util/io.hpp"
#include "util/disassembler.hpp"

namespace ylang {

  address_t Assembly::Write(Token token) {
    if (cursor.address + sizeof(uint64_t) > kHeapMemorySize) {
      printf("Memory overflow\n");
      return offset;
    }

    Value val = Value::CreateValue(token);
    
    address_t addr = cursor;

    uint8_t* data = reinterpret_cast<uint8_t*>(&val); 
    for (uint64_t i = 0; i < sizeof(uint64_t); ++i) {
      memory[cursor.address + i] = data[i];
    }

    ++offset.address;
    cursor.address += sizeof(uint64_t);

    return addr;
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

  const uint8_t* const Assembly::ReadMemory() const {
    return memory;
  }
      
  void Assembly::printchunk(const Chunk* chunk, const std::string& name) {
    printfmt("=== {} ===", name);
    Disassembler::Disassemble(chunk);
    printfmt("=== END {} ===" , name);
  }

  void Assembly::printaddr(address_t addr) {
    uint64_t val = *reinterpret_cast<uint64_t*>(&memory[addr.address]);
    printfmt("{:#08x} = {}", addr , val);
  }

} // namespace ylang

