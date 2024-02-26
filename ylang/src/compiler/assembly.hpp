#ifndef YL_ASSEMBLY_HPP
#define YL_ASSEMBLY_HPP

#include <vector>

#include "defines.hpp"
#include "token.hpp"
#include "value.hpp"
#include "compiler/chunk.hpp"

namespace ylang {

  class Assembly {
    public:
      std::vector<Chunk> chunks;

      address_t Write(Token token);
      Value Read(address_t address);

      void Dump();
      void DumpMemory();

      /// produce the assembly file from the chunks and memory
      template <typename OS>
      OS& WriteTo(OS& os) {
        /// write the memory (data) segment to the file first
        return os;
      }

    private:
      address_t cursor{ 0 };

      void printchunk(const Chunk* chunk, const std::string& name);
      void printaddr(address_t addr);
  };

} // namespace ylang
  //
template <typename OS>
OS& operator<<(OS& os , ylang::Assembly& assembly) {
  return assembly.WriteTo(os);
}

#endif // !YL_ASSEMBLY_HPP
