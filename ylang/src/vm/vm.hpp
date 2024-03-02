#ifndef YL_VM_HPP
#define YL_VM_HPP

#include "vm/register.hpp"
#include "compiler/assembly.hpp"
#include "env/environment.hpp"

namespace ylang {

  class Assembly;
  
  constexpr static uint8_t kRegisterSize = sizeof(Register);
  constexpr static uint32_t kRegisterMemoryOffset = kRegisterSize * kRegisterCount;
  constexpr static uint32_t kNumInstructions = InstructionType::NUM_OP_CODES + 1;

  class VM {
    public:
      VM();
      ~VM();

      void Load(Assembly* assembly);
      void Run();

    private:
      Chunk* current_chunk;
      Instruction* instruction;

      std::vector<Chunk> chunks;

      Register* registers[kRegisterCount];

      void AllocateMemory();
      void AllocateRegisters();

      void ExecuteInstruction();

      Value Read(Operand& op);
      void Write(Operand& op, const Value& val);

      void DumpMemory();

      void ExecUnaryOp(InstructionType type);
      void ExecBinaryOp(InstructionType type);

      void ExecPUSH();
      void ExecPOP();
      void ExecRET();
      void ExecCALL();
      void ExecNEG();
      
      void ExecLEA();

      void ExecADD();
      void ExecSUB();
      void ExecIMUL();
      void ExecDIV();
      void ExecMOV();

      void ExecAND();
      void ExecOR();
      void ExecXOR();
      void ExecNOT();

      void ExecJMP();
  };


} // namespace ylang

#endif // !YL_VM_HPP
