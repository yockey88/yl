#ifndef YL_CHUNK_HPP
#define YL_CHUNK_HPP

#include <optional>
#include <vector>

#include <spdlog/fmt/fmt.h>

#include "defines.hpp"
#include "value.hpp"
#include "util/io.hpp"

namespace ylang {

  struct Operand {
    OperandType type; 
    Value val;

    Operand(OperandType type) : type(type) {}
  };

  struct Instruction {
    InstructionType type;
    std::optional<Operand> lhs;
    std::optional<Operand> rhs;
  };

  class Chunk {
    public:
      Chunk() {}

      std::vector<Instruction> instructions{};
  };

  bool IsUnary(InstructionType type);
  bool IsBinary(InstructionType type);

} // namespace ylang
  
template <> 
struct fmt::formatter<ylang::Operand> : fmt::formatter<std::string_view> {
  auto format(ylang::Operand op , fmt::format_context& ctx) const {
    using ylang::RegisterType;
    using ylang::OperandType;

    std::string val_str = "UNKNOWN";
    switch (op.type) {
      case OperandType::IMMEDIATE: {
        switch (op.val.type) {
          case ylang::Value::ADDRESS: {
            val_str = ylang::fmtstr("{:#08x}" , op.val.As<ylang::address_t>().address);
          } break;
          case ylang::Value::REGISTER: {
            val_str = ylang::fmtstr("{}" , ylang::RegisterStrings[op.val.As<RegisterType>()]);
          } break;
          case ylang::Value::I8: {
            val_str = fmt::format(fmt::runtime("{}") , op.val.As<int8_t>());
          } break;
          case ylang::Value::I16: {
            val_str = ylang::fmtstr("{}" , op.val.As<int16_t>());
          } break;
          case ylang::Value::I32: {
            val_str = ylang::fmtstr("{}" , op.val.As<int32_t>());
          } break;
          case ylang::Value::I64: {
            val_str = ylang::fmtstr("{}" , op.val.As<int64_t>());
          } break;
          case ylang::Value::U8: {
            val_str = fmt::format(fmt::runtime("{}") , op.val.As<uint8_t>());
          } break;
          case ylang::Value::U16: {
            val_str = ylang::fmtstr("{}" , op.val.As<uint16_t>());
          } break;
          case ylang::Value::U32: {
            val_str = ylang::fmtstr("{}" , op.val.As<uint32_t>());
          } break;
          case ylang::Value::U64: {
            val_str = ylang::fmtstr("{}" , op.val.As<uint64_t>());
          } break;
          case ylang::Value::F32: {
            val_str = fmt::format(fmt::runtime("{}") , op.val.As<float>());
          } break;
          case ylang::Value::F64: {
            val_str = fmt::format(fmt::runtime("{}") , op.val.As<double>());
          } break;
          default: break;
        }
      } break;
      case OperandType::REGISTER: {
        val_str = fmt::format(fmt::runtime("{}") , ylang::RegisterStrings[op.val.As<RegisterType>()]);
      } break;
      case OperandType::DIRECT: {
        val_str = fmt::format(fmt::runtime("{:#08x}") , op.val.As<ylang::address_t>().address);
      } break;
      default: break;
    }

    return fmt::formatter<std::string_view>::format(val_str , ctx);
  }
};

#endif // !YL_CHUNK_HPP
