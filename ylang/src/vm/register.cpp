#include "vm/register.hpp"

#include "errors.hpp"

namespace ylang {

  WordSize Register::Size() {
    return value.size;
  }

  Value::Type Register::ValueType() {
    return value.type;
  }

  uint8_t* Register::AsBytes() {
    if (Size() == WordSize::BYTE) {
      return value.AsBytes();
    }
    throw RuntimeError("Register is not a byte");
  }

  int8_t Register::AsSByte() {
    if (Size() == WordSize::BYTE) {
      return value.As<int8_t>();
    }
    throw RuntimeError("Register is not a byte");
  }

  uint8_t Register::AsByte() {
    if (Size() == WordSize::BYTE) {
      return value.As<uint8_t>();
    }
    throw RuntimeError("Register is not a byte");
  }

  int16_t Register::AsSWord() {
    if (Size() == WordSize::WORD) {
      return value.As<int16_t>();
    }
    throw RuntimeError("Register is not a word");
  }
  
  uint16_t Register::AsWord() {
    if (Size() == WordSize::WORD) {
      return value.As<uint16_t>();
    }
    throw RuntimeError("Register is not a word");
  }


  int32_t Register::AsSDWord() {
    if (Size() == WordSize::DWORD) {
      return value.As<int32_t>();
    }
    throw RuntimeError("Register is not a dword");
  }

  uint32_t Register::AsDWord() {
    if (Size() == WordSize::DWORD) {
      return value.As<uint32_t>();
    }
    throw RuntimeError("Register is not a qword");
  }

  int64_t Register::AsSQWord() {
    if (Size() == WordSize::QWORD) {
      return value.As<int64_t>();
    }
    throw RuntimeError("Register is not a qword");
  }

  uint64_t Register::AsQWord() {
    if (Size() == WordSize::QWORD) {
      return value.As<uint64_t>();
    }
    throw RuntimeError("Register is not a qword");
  }

  float Register::AsFloat() {
    if (Size() == WordSize::DWORD) {
      return value.As<float>();
    }
    throw RuntimeError("Register is not a float");
  }

  double Register::AsDouble() {
    if (Size() == WordSize::QWORD) {
      return value.As<double>(); 
    }
    throw RuntimeError("Register is not a double");
  }

  void Register::Write(Value val) {
    value = val;
  }

  Value Register::Read() {
    return value;
  }

} // namespace ylang
