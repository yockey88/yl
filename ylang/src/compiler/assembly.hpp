#ifndef YL_ASSEMBLY_HPP
#define YL_ASSEMBLY_HPP

#include <vector>

#include "defines.hpp"
#include "compiler/chunk.hpp"
#include "env/environment.hpp"

namespace ylang {

  class Assembly {
    public:
      Assembly() 
        : env(new Environment{}) {}
      ~Assembly() {}

      Assembly(Assembly&& other) {
        env = std::move(other.env);
        chunks = std::move(other.chunks);
      }

      Assembly(const Assembly& other) = delete;

      std::vector<Chunk> chunks;

      std::unique_ptr<Environment> Env() { 
        // std::unique_ptr<Environment> ret = env;
        // return ret; 
        return nullptr;
      }

      std::unique_ptr<Environment>& GetEnv() { return env; }

      address_t Write(const std::string& name , Value var);

      address_t ReadAddress(const std::string& name);
      Variable Read(address_t address);
      Variable Read(const std::string& name);

      void Dump();
      void DumpMemory();

      /// produce the assembly file from the chunks and memory
      template <typename OS>
      OS& WriteTo(OS& os) {
        /// write the memory (data) segment to the file first
        return os;
      }

    private:
      std::unique_ptr<Environment> env = nullptr;

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
