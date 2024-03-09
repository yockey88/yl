#include "value.hpp"

#include "errors.hpp"
#include "util/io.hpp"

namespace ylang {
      
  void Value::WriteSValue(Value& value , const int64_t& val) {
    switch (value.size) {
      case BYTE:
        value.type = I8;
        value.value = static_cast<int8_t>(val);
      break;
      case WORD:
        value.type = I16;
        value.value = static_cast<int16_t>(val);
      break;
      case DWORD:
        value.type = I32;
        value.value = static_cast<int32_t>(val);
      break;
      case QWORD:
        value.type = I64;  
        value.value = static_cast<int64_t>(val);
      break;
      default:
        throw RuntimeError("Unknown size in WriteSValue");
    }
  }

  void Value::WriteUValue(Value& value , const uint64_t& val) {
    switch (value.size) {
      case BYTE:
        value.type = U8;
        value.value = static_cast<uint8_t>(val);
      break;
      case WORD:
        value.type = U16;
        value.value = static_cast<uint16_t>(val);
      break;
      case DWORD:
        value.type = U32;
        value.value = static_cast<uint32_t>(val);
      break;
      case QWORD:
        value.type = U64;
        value.value = static_cast<uint64_t>(val);
      break;
      default:
        throw RuntimeError("Unknown size in WriteUValue");
    }
  }

  uint8_t* Value::AsBytes() {
    return std::visit([](auto&& arg) -> uint8_t* {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , std::monostate>) {
        return nullptr;
      } else {
        return reinterpret_cast<uint8_t*>(&arg);
      }
    }, value);
  }

  int64_t Value::AsSInt() const {
    return std::visit([](auto&& arg) -> int64_t {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg ? 1 : 0;
      } else if constexpr (std::is_same_v<T , char>) {
        return arg;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return arg;
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return static_cast<int64_t>(arg);
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return static_cast<int64_t>(arg);
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return static_cast<int64_t>(arg);
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return static_cast<int64_t>(arg);
      } else if constexpr (std::is_same_v<T, float>) {
        return static_cast<int64_t>(arg);
      } else if constexpr (std::is_same_v<T, double>) {
        return static_cast<int64_t>(arg);
      } else if constexpr (std::is_same_v<T, bool>) {
        return arg ? 1 : 0;
      } else if constexpr (std::is_same_v<T, address_t> || std::is_same_v<T , RegisterType>) {
        throw RuntimeError("Cannot convert address to int"); 
      } else if constexpr (std::is_same_v<T , std::monostate>){
        return 0;
      } else {
        throw RuntimeError("Unknown type in AsSInt");
      }
    }, value);
  }

  uint64_t Value::AsUInt() const {
    return std::visit([](auto&& arg) -> uint64_t {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg ? 1 : 0;
      } else if constexpr (std::is_same_v<T , char>) {
        return arg;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return static_cast<uint64_t>(arg);
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return static_cast<uint64_t>(arg);
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return static_cast<uint64_t>(arg);
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return static_cast<uint64_t>(arg);
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T, float>) {
        return static_cast<uint64_t>(arg);
      } else if constexpr (std::is_same_v<T, double>) {
        return static_cast<uint64_t>(arg);
      } else if constexpr (std::is_same_v<T, bool>) {
        return arg ? 1 : 0;
      } else if constexpr (std::is_same_v<T, address_t>) {
        return arg.address;
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        return arg;
      } else if constexpr (std::is_same_v<T , std::monostate>){
        return 0;
      } else {
        throw RuntimeError("Unknown type in AsUInt");
      }
    }, value);
  }

  float Value::AsFloat() const {
    return std::visit([](auto&& arg) -> float {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg ? 1.0f : 0.0f;
      } else if constexpr (std::is_same_v<T , char>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, float>) {
        return arg;
      } else if constexpr (std::is_same_v<T, double>) {
        return static_cast<float>(arg);
      } else if constexpr (std::is_same_v<T, bool>) {
        return arg ? 
          1.0f : 0.0f;
      } else if constexpr (std::is_same_v<T, address_t> || std::is_same_v<T , RegisterType>) {
        throw RuntimeError("Cannot convert register to float");
      } else if constexpr (std::is_same_v<T , std::monostate>){
        return 0.0f;
      } else {
        throw RuntimeError("Unknown type in AsFloat");
      }
    }, value);
  }

  double Value::AsDouble() const {
    return std::visit([](auto&& arg) -> double {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg ? 1.0 : 0.0;
      } else if constexpr (std::is_same_v<T , char>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, float>) {
        return static_cast<double>(arg);
      } else if constexpr (std::is_same_v<T, double>) {
        return arg;
      } else if constexpr (std::is_same_v<T, bool>) {
        return arg ? 1.0 : 0.0;
      } else if constexpr (std::is_same_v<T, address_t> || std::is_same_v<T , RegisterType>) {
        throw RuntimeError("Cannot convert register to double");
      } else if constexpr (std::is_same_v<T , std::monostate>){
        return 0.0;
      } else {
        throw RuntimeError("Unknown type in AsDouble");
      }
    }, value);
  }

  address_t Value::AsAddress() const {
    return std::visit([](auto&& arg) -> address_t {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg ? 1 : 0;
      } else if constexpr (std::is_same_v<T , char>) {
        return address_t{static_cast<uint64_t>(arg)};
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return address_t{static_cast<uint64_t>(arg)};
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return address_t{static_cast<uint64_t>(arg)};
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return address_t{static_cast<uint64_t>(arg)};
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return address_t{static_cast<uint64_t>(arg)};
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return address_t{arg};
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return address_t{arg};
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return address_t{arg};
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return address_t{arg};
      } else if constexpr (std::is_same_v<T, float>) {
        throw RuntimeError("Cannot convert float to address");
      } else if constexpr (std::is_same_v<T, double>) {
        throw RuntimeError("Cannot convert double to address");
      } else if constexpr (std::is_same_v<T, bool>) {
        return address_t{arg ? 1 : 0};
      } else if constexpr (std::is_same_v<T, address_t>) {
        return arg;
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        throw RuntimeError("Cannot convert register to address");
      } else if constexpr (std::is_same_v<T , std::monostate>){
        return address_t{0};
      } else {
        throw RuntimeError("Unknown type in AsAddress");
      }
    }, value);
  }

  RegisterType Value::AsRegister() const {
    return std::visit([](auto&& arg) -> RegisterType {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , RegisterType>) {
        return arg;
      } else {
        return RegisterType{0};
      }
    }, value);
  }

  bool Value::AsBool() const {
    return std::visit([](auto&& arg) -> bool {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg;
      } else if constexpr (std::is_same_v<T , char>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return arg != 0;
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, float>) {
        return arg != 0.0f;
      } else if constexpr (std::is_same_v<T, double>) {
        return arg != 0.0;
      } else if constexpr (std::is_same_v<T, bool>) {
        return arg;
      } else if constexpr (std::is_same_v<T, address_t>) {
        return arg.address != 0;
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        return static_cast<uint8_t>(arg) != 0;
      } else if constexpr (std::is_same_v<T , std::monostate>){
        return false;
      } else {
        throw RuntimeError("Unknown type in AsBool");
      }
    }, value);
  }

  char Value::AsChar() const {
    return std::visit([](auto&& arg) -> char {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg ? 1 : 0;
      } else if constexpr (std::is_same_v<T , char>) {
        return arg;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return static_cast<char>(arg);
      } else if constexpr (std::is_same_v<T, bool>) {
        return arg ? 1 : 0;
      } else if constexpr (std::is_same_v<T , std::monostate>){
        return 0;
      } else {
        throw RuntimeError("Unknown type in AsChar");
      }
    }, value);
  }

  std::string Value::AsString() const {
    std::string result;
    std::visit([&result](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        result = arg ? "true" : "false";
      } else if constexpr (std::is_same_v<T , char>) {
        result = std::string(1 , arg);
      } else if constexpr (std::is_same_v<T , int8_t>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, int16_t>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, int32_t>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, int64_t>) {
         result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, float>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, double>) {
        result = std::to_string(arg);
      } else if constexpr (std::is_same_v<T, address_t>) {
        result = fmtstr("[{:#08x}]" , arg.address);
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        result = RegisterStrings[arg];
      } else if constexpr (std::is_same_v<T , std::monostate>){
        result = "nil";
      } else {
        throw RuntimeError("Unknown type in AsString");
      }
    }, value);
    return result;
  }

  Value::Value(Value&& rhs) 
    : type(rhs.type) , size(rhs.size) , value(rhs.value) {
  }

  Value::Value(const Value& rhs) 
    : type(rhs.type) , size(rhs.size) , value(rhs.value) {
  }

  Value& Value::operator=(Value&& rhs) {
    type = rhs.type;
    size = rhs.size;
    value = rhs.value;
    return *this;
  }

  Value& Value::operator=(const Value& rhs) {
    type = rhs.type;
    size = rhs.size;
    value = rhs.value;
    return *this;
  }

  void Value::Zero() {
    return std::visit([](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        arg = false;
      } else if constexpr (std::is_same_v<T , char>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T, int16_t>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T, int32_t>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T, int64_t>) {
         arg = 0;
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        arg = 0;
      } else if constexpr (std::is_same_v<T, float>) {
        arg = 0.0f;
      } else if constexpr (std::is_same_v<T, double>) {
        arg = 0.0;
      } else if constexpr (std::is_same_v<T, bool>) {
        arg = false;
      } else if constexpr (std::is_same_v<T, address_t>) {
        arg.address = 0;
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        arg = static_cast<RegisterType>(0);
      } else if constexpr (std::is_same_v<T , std::monostate>){
        // do nothing
      } else {
        throw RuntimeError("Unknown type in Zero");
      }
    }, value);
  }

  void Value::Negate() {
    return std::visit([](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        arg = !arg;
      } else if constexpr (std::is_same_v<T , char>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, int16_t>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, int32_t>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, int64_t>) {
         arg = -arg;
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, float>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, double>) {
        arg = -arg;
      } else if constexpr (std::is_same_v<T, bool>) {
        arg = !arg;
      } else if constexpr (std::is_same_v<T, address_t>) {
        throw RuntimeError("Cannot negate address");
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        throw RuntimeError("Cannot negate register");
      } else if constexpr (std::is_same_v<T , std::monostate>){
        // do nothing
      } else {
        throw RuntimeError("Unknown type in Negate");
      }
    }, value);
  }

  bool Value::IsNull() const {
    return std::visit([](auto&& arg) -> bool {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, address_t>) {
        return arg.address == 0;
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        return static_cast<uint8_t>(arg) == 0;
      } else if constexpr (std::is_same_v<T , std::monostate>) {
        return true;
      } else {
        return false;
      }
    } , value);
  }

  bool Value::IsNegative() const {
    return std::visit([](auto&& arg) -> bool {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return false;
      } else if constexpr (std::is_same_v<T , char>) {
        return arg < 0;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return arg < 0;
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return arg < 0;
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return arg < 0;
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return arg < 0;
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return false;
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return false;
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return false;
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return false;
      } else if constexpr (std::is_same_v<T, float>) {
        return arg < 0.0f;
      } else if constexpr (std::is_same_v<T, double>) {
        return arg < 0.0;
      } else if constexpr (std::is_same_v<T, address_t>) {
        return arg.address < 0;
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        return static_cast<uint8_t>(arg) < 0;
      } else if constexpr (std::is_same_v<T , std::monostate>) {
        return false;
      } else {
        throw RuntimeError("Unknown type in IsNegative");
      }
    } , value);
  }

  bool Value::IsZero() const {
    return std::visit([](auto&& arg) -> bool {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        return arg == false;
      } else if constexpr (std::is_same_v<T , char>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T , int8_t>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T, int16_t>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T, int32_t>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T, int64_t>) {
         return arg == 0;
      } else if constexpr (std::is_same_v<T, uint8_t>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T, uint16_t>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T, uint32_t>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T, uint64_t>) {
        return arg == 0;
      } else if constexpr (std::is_same_v<T, float>) {
        return arg == 0.0f;
      } else if constexpr (std::is_same_v<T, double>) {
        return arg == 0.0;
      } else if constexpr (std::is_same_v<T, address_t>) {
        return arg.address == 0;
      } else if constexpr (std::is_same_v<T , RegisterType>) {
        return static_cast<uint8_t>(arg) == 0;
      } else if constexpr (std::is_same_v<T , std::monostate>) {
        return true;
      } else {
        throw RuntimeError("Unknown type in IsZero");
      }
    } , value);
  }

  bool Value::IsUnsigned() const {
    if (type == Type::U8 || type == Type::U16 || type == Type::U32 || type == Type::U64) {
      return true;
    }

    return false;
  }

  bool Value::IsSigned() const {
    if (type == Type::I8 || type == Type::I16 || type == Type::I32 || type == Type::I64) {
      return true;
    }

    return false;
  }

  bool Value::IsInt() const {
    return IsSigned() || IsUnsigned();
  }

  bool Value::IsFloat() const {
    if (type == Type::F32 || IsInt()) {
      return true;
    }

    return false;
  }

  bool Value::IsDouble() const {
    if (type == Type::F64 || IsFloat()) {
      return true;
    }

    return false;
  }

  bool Value::IsBool() const {
    return type == Type::BOOL;
  }

  bool Value::IsChar() const {
    return type == Type::CHAR;
  }

  bool Value::IsAddress() const {
    return type == Type::ADDRESS;
  }

  bool Value::IsRegister() const {
    return type == Type::REGISTER;
  }
      
  bool Value::IsString() const {
    return type == Type::STRING;
  }

  bool Value::IsArray() const {
    return type == Type::ARRAY;
  }

  bool Value::TypesCompatible(const Value& rhs) const {
    if (type == rhs.type) {
      return true;
    }

    if (IsDouble() && rhs.IsDouble()) {
      return true;
    }

    if ((IsAddress() || IsRegister()) && (rhs.IsAddress() || rhs.IsRegister())) {
      return true;
    }

    return false;
  }

  bool Value::TypesCompatible(Value::Type lhs , Value::Type rhs) {
    return GetCommonType(lhs , rhs) != Type::VOID;
  }
      
  Value::Type Value::GetType(const Token& token) {
    switch (token.type) {
      case INTEGER: {
        return Value::Type::U32;
      }
      case TokenType::SINTEGER: {
        return Value::Type::I32;
      }
      case TokenType::I8:
        return Value::Type::I8;
      case TokenType::U8:
        return Value::Type::U8;
      case TokenType::I16:
        return Value::Type::I16;
      case TokenType::U16:
        return Value::Type::U16;
      case TokenType::I32:
        return Value::Type::I32;
      case TokenType::U32:
        return Value::Type::U32;
      case TokenType::I64:
        return Value::Type::I64;
      case TokenType::U64:
        return Value::Type::U64;
      case TokenType::F32:
        return Value::Type::F32;
      case TokenType::F64:
        return Value::Type::F64;
      case TokenType::CHAR:
        return Value::Type::CHAR;
      case TokenType::BOOL:
        return Value::Type::BOOL;
      case TokenType::ADDRESS:
        return Value::Type::ADDRESS;
      case TokenType::STRING:
        return Value::Type::STRING;
      case TokenType::IDENTIFIER:
        return Value::Type::STRUCT;
      case TokenType::UNKNOWN:
        return Value::Type::VOID;
      default:
        return Value::Type::NIL;
    }
  }
      
  TokenType Value::GetTokenType(Value::Type type) {
    switch (type) {
      case Value::Type::I8:
        return TokenType::I8;
      case Value::Type::U8:
        return TokenType::U8;
      case Value::Type::I16:
        return TokenType::I16;
      case Value::Type::U16:
        return TokenType::U16;
      case Value::Type::I32:
        return TokenType::I32;
      case Value::Type::U32:
        return TokenType::U32;
      case Value::Type::I64:
        return TokenType::I64;
      case Value::Type::U64:
        return TokenType::U64;
      case Value::Type::F32:
        return TokenType::F32;
      case Value::Type::F64:
        return TokenType::F64;
      case Value::Type::CHAR:
        return TokenType::CHAR;
      case Value::Type::BOOL:
        return TokenType::BOOL;
      case Value::Type::CALLABLE:
      case Value::Type::ADDRESS:
        return TokenType::ADDRESS;
      case Value::Type::STRING:
        return TokenType::STRING;
      case Value::Type::NIL:
        return TokenType::NIL;
      case Value::Type::VOID:
        return TokenType::UNKNOWN;
      default:
        return TokenType::UNKNOWN;
    }
  }

  Value Value::CreateValue(const Token& token) {
    Value value;
    switch (token.type) {
      case TokenType::I8: {
        value.size = BYTE;
        value.type = Type::I8;
        value.value = 0;
        return value;
      }

      case TokenType::U8: {
        value.size = BYTE;
        value.type = Type::U8;
        value.value = 0;
        return value;
      }

      case TokenType::I16: {
        value.size = WORD;
        value.type = Type::I16;
        value.value = 0;
        return value;
      }

      case TokenType::U16: {
        value.size = WORD;
        value.type = Type::U16;
        value.value = 0;
        return value;
      }

      case TokenType::I32: {
        value.size = DWORD;
        value.type = Type::I32;
        value.value = 0;
        return value;
      }

      case TokenType::U32: {
        value.size = DWORD;
        value.type = Type::U32;
        value.value = 0;
        return value;
      }

      case TokenType::I64: {
        value.size = QWORD;
        value.type = Type::I64;
        value.value = 0;
        return value;
      }

      case TokenType::U64: {
        value.size = QWORD;
        value.type = Type::U64;
        value.value = 0;
        return value;
      }

      case TokenType::F32: {
        value.size = DWORD;
        value.type = Type::F32;
        value.value = 0.0f;
        return value;
      }

      case TokenType::F64: {
        value.size = QWORD;
        value.type = Type::F64;
        value.value = 0.0;
        return value;
      }

      case TokenType::TRUE:
      case TokenType::FALSE:
      case TokenType::BOOL: {
        value.size = BYTE;
        value.type = Type::BOOL;
        Write(value, token.value == "true");
        return value;
      }

      case TokenType::CHARL:
      case TokenType::CHAR: {
        value.size = BYTE;
        value.type = Type::CHAR;
        Write(value, token.value[0]);
        return value;
      }

      case SINTEGER: {
        int64_t int_value = std::stoll(token.value);
        if (int_value >= std::numeric_limits<int8_t>::min() && int_value <= std::numeric_limits<int8_t>::max()) {
          value.size = BYTE;
          value.type = Type::I8;
          Write(value, static_cast<int8_t>(int_value));
        } else if (int_value >= std::numeric_limits<int16_t>::min() && int_value <= std::numeric_limits<int16_t>::max()) {
          value.size = WORD;
          value.type = Type::I16;
          Write(value, static_cast<int16_t>(int_value));
        } else if (int_value >= std::numeric_limits<int32_t>::min() && int_value <= std::numeric_limits<int32_t>::max()) {
          value.size = DWORD;
          value.type = Type::I32;
          Write(value, static_cast<int32_t>(int_value));
        } else {
          value.size = QWORD;
          value.type = Type::I64;
          Write(value, int_value);
        }
        return value;
      }

      case INTEGER: {
        uint64_t int_value = std::stoull(token.value);
        if (int_value <= std::numeric_limits<uint8_t>::max()) {
          value.size = BYTE;
          value.type = Type::U8; 
          Write(value, static_cast<uint8_t>(int_value)); 
        } else if (int_value <= std::numeric_limits<uint16_t>::max()) { 
          value.size = WORD;
          value.type = Type::U16;
          Write(value, static_cast<uint16_t>(int_value));
        } else if (int_value <= std::numeric_limits<uint32_t>::max()) {
          value.size = DWORD;
          value.type = Type::U32;
          Write(value, static_cast<uint32_t>(int_value));
        } else {
          value.size = QWORD;
          value.type = Type::U64;
          Write(value, int_value);
        }
        return value;
      }

      case FLOAT: {
        float float_value = std::stof(token.value);
        value.size = DWORD;
        value.type = Type::F32;
        Write(value, float_value);
        return value;
      }

      case DOUBLE: {
        double double_value = std::stod(token.value);
        value.size = QWORD;
        value.type = Type::F64;
        Write(value, double_value);
        return value;
      }

      case STRINGL: {
        value.size = QWORD;
        value.type = Type::STRING;
        return value;
      }
      case TokenType::ADDRESS: {
        value.size = QWORD;
        value.type = Type::ADDRESS;
        address_t address = std::stoull(token.value);
        Write(value, address);
        return value;
      }

      case TokenType::IDENTIFIER: {
        value.size = BYTE;
        value.type = Type::ADDRESS;
        Write(value , 0);
        return value;
      }

      default:
        throw RuntimeError(fmtstr("Invalid token type {} for {}" , TokenTypeStrings[token.type] , token.value));
    }
  }
      
  Value Value::CreateValue(bool val) {
    Value v;
    v.size = BYTE;
    v.type = Type::BOOL;
    Write(v, val);
    return v;
  }
      
  Value Value::CreateValue(char val) {
    Value v;
    v.size = BYTE;
    v.type = Type::CHAR;
    Write(v, val);
    return v;
  }

  Value Value::CreateValue(const address_t& value) {
    Value v;
    v.size = QWORD;
    v.type = Type::ADDRESS;
    Write(v, value);
    return v;
  }

  Value Value::CreateValue(RegisterType value) {
    Value v;
    v.size = QWORD;
    v.type = Type::REGISTER;
    Write(v, value);
    return v;
  }

  Value Value::CreateNilValue(Value::Type type) {
    Value v;
    v.size = GetTypeWordSize(type);
    v.type = type;
    v.value = std::monostate{};
    return v;
  }

  size_t Value::GetTypeSize(Value::Type type) {
    switch (type) {
      case Type::BOOL:
      case Type::CHAR:
      case Type::I8:
      case Type::U8:
        return BYTE;

      case Type::I16:
      case Type::U16:
        return WORD;

      case Type::I32:
      case Type::U32:
      case Type::F32:
        return DWORD;

      case Type::I64:
      case Type::U64:
      case Type::F64:
      case Type::ADDRESS:
        return QWORD;

      default:
        return 0;
    }
  }

  WordSize Value::GetTypeWordSize(Value::Type type) {
    switch (type) {
      case Type::BOOL:
      case Type::CHAR:
      case Type::I8:
      case Type::U8:
        return BYTE;

      case Type::I16:
      case Type::U16:
        return WORD;

      case Type::I32:
      case Type::U32:
      case Type::F32:
        return DWORD;

      case Type::I64:
      case Type::U64:
      case Type::F64:
      case Type::ADDRESS:
        return QWORD;

      default:
        return UNKNOWN_SIZE;
    }
  }

  Value::Type GetCommonType(const Value& lhs , const Value& rhs) {
    if (lhs.IsSigned()) {
      if (rhs.IsUnsigned()) {
        return std::abs(lhs.AsSInt()) > rhs.AsUInt() ? 
          lhs.type : rhs.type;
      } 

      if (rhs.IsNegative()) {
        return std::abs(rhs.AsSInt()) > std::abs(lhs.AsSInt()) ?
          rhs.type : lhs.type;
      }
    }

    if (lhs.IsUnsigned()) {
      if (rhs.IsUnsigned()) {
        return rhs > lhs ?
          rhs.type : lhs.type;
      }

      if (rhs.IsNegative()) {
        return std::abs(rhs.AsSInt()) > lhs.AsUInt() ?
          rhs.type : lhs.type;
      }
    }

    if (lhs.IsDouble()) {
      if (rhs.IsInt()) {
        return lhs.type;
      }

      return lhs > rhs ? lhs.type : rhs.type;
    }

    if (lhs.IsAddress() || lhs.IsRegister()) {
      if (rhs.IsAddress() || rhs.IsRegister()) {
        return lhs.type;
      } else {
        throw RuntimeError("No type conversion from address/register to non-address/register type");
      }
    }

    throw RuntimeError(fmtstr("No common type found {} and {}" , lhs , rhs));
  }
      
  Value::Type Value::GetCommonType(Value::Type lhs , Value::Type rhs) {
    switch (lhs) {
      case BOOL:
      case CHAR:
        if (rhs != BOOL && rhs != CHAR && rhs != U8) {
          return Type::VOID;
        }
        return lhs;

      case I8:
      case U8: {
        if (rhs != I8 && rhs != I16 && rhs != I32 && rhs != I64 && 
            rhs != U8 && rhs != U16 && rhs != U32 && rhs != U64) {
          return Type::VOID;
        }
        if (rhs == U16 || rhs == U32 || rhs == U64 ||
            rhs == I16 || rhs == I32 || rhs == I64) {
          return rhs;
        }
        return lhs;
      }

      case I16:
      case U16: {
        if (rhs != I8 && rhs != I16 && rhs != I32 && rhs != I64 && 
            rhs != U8 && rhs != U16 && rhs != U32 && rhs != U64) {
          return Type::VOID;
        }
        if (rhs == U32 || rhs == U64 ||
            rhs == I32 || rhs == I64) {
          return rhs;
        }
        return lhs;
      }

      case I32:
      case U32: {
        if (rhs != I8 && rhs != I16 && rhs != I32 && rhs != I64 && 
            rhs != U8 && rhs != U16 && rhs != U32 && rhs != U64) {
          return Type::VOID;
        }
        if (rhs == U64 || rhs == I64) {
          return rhs;
        }
        return lhs;
      }

      case I64: {
        if (rhs != I8 && rhs != I16 && rhs != I32 && rhs != I64 &&
            rhs != U8 && rhs != U16 && rhs != U32 && rhs != U64) {
          return Type::VOID;
        }
        return lhs;
      }

      case U64: {
        if (rhs != I8 && rhs != I16 && rhs != I32 && rhs != I64 && 
            rhs != U8 && rhs != U16 && rhs != U32 && rhs != U64 && rhs != ADDRESS) {
          return Type::VOID;
        }
        return lhs;
      }

      case F32:
      case F64: {
        if (rhs != F32 && rhs != F64 && 
            rhs != I8 && rhs != I16 && rhs != I32 && rhs != I64 &&
            rhs != U8 && rhs != U16 && rhs != U32 && rhs != U64) {
          return Type::VOID;
        }
        return lhs;
      }

      case ARRAY:
      case STRING: {
        if (rhs != STRING && rhs != ARRAY && rhs != ADDRESS) {
          return Type::VOID;
        }
        return lhs;
      }
      case ADDRESS:
        if (rhs != ADDRESS && rhs != U64) {
          return Type::VOID;
        }
        return lhs;
      default:
        return Type::VOID;
    }
  }
      
  WordSize Value::ResolveSWordSize(int64_t size) {
    if (size >= std::numeric_limits<int8_t>::min() && size <= std::numeric_limits<int8_t>::max()) {
      return BYTE;
    } else if (size >= std::numeric_limits<int16_t>::min() && size <= std::numeric_limits<int16_t>::max()) {
      return WORD;
    } else if (size >= std::numeric_limits<int32_t>::min() && size <= std::numeric_limits<int32_t>::max()) {
      return DWORD;
    } else {
      return QWORD;
    }
  }
  
  WordSize Value::ResolveUWordSize(uint64_t size) {
    if (size <= std::numeric_limits<uint8_t>::max()) {
      return BYTE;
    } else if (size <= std::numeric_limits<uint16_t>::max()) {
      return WORD;
    } else if (size <= std::numeric_limits<uint32_t>::max()) {
      return DWORD;
    } else {
      return QWORD;
    }
  }

  Value::Type Value::UnsignedToSigned(Value::Type type) {
    switch (type) {
      case Type::I8:
      case Type::I16:
      case Type::I32:
      case Type::I64:
        return type;

      case Type::U8:
        return Type::I8;

      case Type::U16:
        return Type::I16;

      case Type::U32:
        return Type::I32;

      case Type::U64:
        return Type::I64;

      default:
        throw RuntimeError("Invalid type for UnsignedToSigned");
    }
  }

  Value::Type Value::SignedToUnsigned(Value::Type type) {
    switch (type) {
      case Type::I8:
        return Type::U8;

      case Type::I16:
        return Type::U16;

      case Type::I32:
        return Type::U32;

      case Type::I64:
        return Type::U64;

      default:
        throw RuntimeError("Invalid type for SignedToUnsigned");
    }
  }

  bool operator<(const address_t& lhs, const Value& rhs) {
    if (rhs.IsDouble() || rhs.IsBool()) {
      throw RuntimeError("Invalid comparison , between address type and non-address type");
    }

    return lhs < rhs.AsAddress();
  }

  bool operator>(const address_t& lhs, const Value& rhs) {
    return !(lhs <= rhs);
  }

  bool operator==(const address_t& lhs, const Value& rhs) {
    if (rhs.IsDouble() || rhs.IsBool()) {
      throw RuntimeError("Invalid comparison , between address type and non-address type");
    }

    return lhs == rhs.AsAddress();
  }

  bool operator!=(const address_t& lhs, const Value& rhs) {
    return !(lhs == rhs);
  }

  bool operator<=(const address_t& lhs, const Value& rhs) {
    return lhs < rhs || lhs == rhs;
  }

  bool operator>=(const address_t& lhs, const Value& rhs) {
    return !(lhs < rhs);
  }

  address_t operator+(const address_t& lhs, const Value& rhs) {
    if (rhs.IsDouble() || rhs.IsBool()) {
      throw RuntimeError("Invalid operation , between address type and non-address type");
    }

    return lhs + rhs.AsAddress();
  }

  address_t operator-(const address_t& lhs, const Value& rhs) {
    if (rhs.IsDouble() || rhs.IsBool()) {
      throw RuntimeError("Invalid operation , between address type and non-address type");
    }

    return lhs - rhs.AsAddress();
  }

  address_t operator+(const Value& lhs, const address_t& rhs) {
    if (lhs.IsDouble() || lhs.IsBool()) {
      throw RuntimeError("Invalid operation , between address type and non-address type");
    }

    return lhs.AsAddress() + rhs;
  }

  address_t operator-(const Value& lhs, const address_t& rhs) {
    if (lhs.IsDouble() || lhs.IsBool()) {
      throw RuntimeError("Invalid operation , between address type and non-address type");
    }

    return lhs.AsAddress() - rhs;
  }

  bool operator<(const Value& lhs , const Value& rhs) {
    if (!lhs.TypesCompatible(rhs)) {
      throw RuntimeError("Invalid comparison , between incompatible types");
    }

    if (lhs.IsDouble()) {
      return lhs.AsDouble() < rhs.AsDouble();
    }

    if (lhs.IsUnsigned()) {
      return lhs.AsUInt() < rhs.AsUInt();
    }

    if (lhs.IsSigned()) {
      return lhs.AsSInt() < rhs.AsSInt();
    }

    if (lhs.IsAddress() || lhs.IsRegister()) {
      return lhs.AsRegister() < rhs.AsRegister();
    }

    throw RuntimeError("Invalid < comparison between values");
  }

  bool operator>(const Value& lhs , const Value& rhs) {
    return !(lhs <= rhs);
  }

  bool operator==(const Value& lhs , const Value& rhs) {
    if (!lhs.TypesCompatible(rhs)) {
      throw RuntimeError("Invalid comparison , between incompatible types");
    }

    if (lhs.IsDouble()) {
      return lhs.AsDouble() == rhs.AsDouble();
    }

    if (lhs.IsUnsigned()) {
      return lhs.AsUInt() == rhs.AsUInt();
    }

    if (lhs.IsSigned()) {
      return lhs.AsSInt() == rhs.AsSInt();
    }

    if (lhs.IsAddress() || lhs.IsRegister()) {
      return lhs.AsRegister() == rhs.AsRegister();
    }

    if (lhs.IsChar() && (rhs.IsChar() || rhs.IsUnsigned())) {
      return lhs.AsChar() == rhs.AsChar();
    }

    throw RuntimeError("Invalid == comparison between values");
  }

  bool operator!=(const Value& lhs , const Value& rhs) {
    return !(lhs == rhs);
  }

  bool operator<=(const Value& lhs , const Value& rhs) {
    return lhs < rhs || lhs == rhs;
  }

  bool operator>=(const Value& lhs , const Value& rhs) {
    return !(lhs < rhs);
  }

  bool operator&&(const Value& lhs , const Value& rhs) {
    return lhs.AsBool() && rhs.AsBool();
  }

  bool operator||(const Value& lhs , const Value& rhs) {
    return lhs.AsBool() || rhs.AsBool();
  }

  Value operator+(const Value& lhs , const Value& rhs) {
    if (!lhs.TypesCompatible(rhs)) {
      throw RuntimeError("Invalid operation , between incompatible types");
    }

    Value v;
    v.type = GetCommonType(lhs, rhs);
    switch (v.type) {
      case Value::Type::BOOL: {
        bool result = lhs.AsBool() + rhs.AsBool();
        Value::Write(v , result);
        return v;
      }

      case Value::Type::CHAR: {
        throw RuntimeError("Invalid + operation between char values");
      }

      case Value::Type::I8:
      case Value::Type::I16:
      case Value::Type::I32:
      case Value::Type::I64: {
        int64_t result = lhs.AsSInt() + rhs.AsSInt();
        v.size = Value::ResolveSWordSize(result);
        Value::WriteSValue(v , result);
        return v;
      }

      case Value::Type::U8:
      case Value::Type::U16:
      case Value::Type::U32:
      case Value::Type::U64: {
        uint64_t result = lhs.AsUInt() + rhs.AsUInt();
        v.size = Value::ResolveUWordSize(result);
        Value::WriteUValue(v , result);
        return v;
      }

      case Value::Type::F32: {
        float result = lhs.AsFloat() + rhs.AsFloat();
        v.size = DWORD;
        Value::Write(v , result);
        return v;
      }

      case Value::Type::F64: {
        double result = lhs.AsDouble() + rhs.AsDouble();
        v.size = QWORD;
        Value::Write(v , result);
        return v;
      }
      
      case Value::Type::ADDRESS: 
      case Value::Type::REGISTER: {
        address_t result = lhs.AsAddress() + rhs.AsAddress();
        v.size = QWORD;
        Value::Write(v , result);
        return v;
      }

      case Value::Type::ARRAY: {
        throw RuntimeError("Invalid + operation between array values");
      }

      default:
        throw RuntimeError("Invalid + operation between values");
    }

    throw RuntimeError("Invalid + operation between values");
  }

  Value operator-(const Value& lhs , const Value& rhs) {
    if (!lhs.TypesCompatible(rhs)) {
      throw RuntimeError("Invalid operation , between incompatible types");
    }

    Value v;
    v.type = GetCommonType(lhs, rhs);
    
    if (rhs > lhs && v.IsUnsigned()) {
      v.type = Value::UnsignedToSigned(v.type);
    }

    switch (v.type) {
      case Value::Type::BOOL: {
        bool result = lhs.AsBool() - rhs.AsBool();
        Value::Write(v , result);
        return v;
      }

      case Value::Type::CHAR: {
        throw RuntimeError("Invalid - operation between char values");
      }

      case Value::Type::I8:
      case Value::Type::I16:
      case Value::Type::I32:
      case Value::Type::I64: {
        int64_t result = lhs.AsSInt() - rhs.AsSInt();
        v.size = Value::ResolveSWordSize(result);
        Value::WriteSValue(v , result);
        return v;
      }

      case Value::Type::U8:
      case Value::Type::U16:
      case Value::Type::U32:
      case Value::Type::U64: {
        uint64_t result = lhs.AsUInt() - rhs.AsUInt();
        v.size = Value::ResolveUWordSize(result);
        Value::WriteUValue(v , result);
        return v;
      }

      case Value::Type::F32: {
        float result = lhs.AsFloat() - rhs.AsFloat();
        Value::Write(v , result);
        return v;
      }

      case Value::Type::F64: {
        double result = lhs.AsDouble() - rhs.AsDouble();
        Value::Write(v , result);
        return v;
      }
      
      case Value::Type::ADDRESS: 
      case Value::Type::REGISTER: {
        address_t result = lhs.AsAddress() - rhs.AsAddress();
        Value::Write(v , result);
        return v;
      }

      default:
        throw RuntimeError("Invalid - operation between values");
    }

    throw RuntimeError("Invalid - operation between values");
  }

  Value operator*(const Value& lhs , const Value& rhs) {
    if (!lhs.TypesCompatible(rhs)) {
      throw RuntimeError("Invalid operation , between incompatible types");
    }

    Value v;
    v.type = GetCommonType(lhs, rhs);

    switch (v.type) {
      case Value::Type::BOOL: {
        bool result = lhs.AsBool() * rhs.AsBool();
        Value::Write(v , result);
        return v;
      }

      case Value::Type::CHAR: {
        throw RuntimeError("Invalid * operation between char values");
      }

      case Value::Type::I8:
      case Value::Type::I16:
      case Value::Type::I32:
      case Value::Type::I64: {
        int64_t result = lhs.AsSInt() * rhs.AsSInt();
        v.size = Value::ResolveSWordSize(result);
        Value::WriteSValue(v , result);
        return v;
      }

      case Value::Type::U8:
      case Value::Type::U16:
      case Value::Type::U32:
      case Value::Type::U64: {
        uint64_t result = lhs.AsUInt() * rhs.AsUInt();
        v.size = Value::ResolveUWordSize(result);
        Value::WriteUValue(v , result);
        return v;
      }

      case Value::Type::F32: {
        float result = lhs.AsFloat() * rhs.AsFloat();
        Value::Write(v , result);
        return v;
      }

      case Value::Type::F64: {
        double result = lhs.AsDouble() * rhs.AsDouble();
        Value::Write(v , result);
        return v;
      }

      default:
        throw RuntimeError("Invalid * operation between values");
    }

    throw RuntimeError("Invalid * operation between values");
  }

  Value operator/(const Value& lhs , const Value& rhs) {
    if (!lhs.TypesCompatible(rhs)) {
      throw RuntimeError("Invalid operation , between incompatible types");
    }

    if (rhs.IsZero()) {
      throw RuntimeError("Division by zero");
    }

    Value v;
    v.type = GetCommonType(lhs, rhs);

    switch (v.type) {
      case Value::Type::BOOL: {
        bool result = lhs.AsBool() / rhs.AsBool();
        Value::Write(v , result);
        return v;
      }

      case Value::Type::CHAR: {
        throw RuntimeError("Invalid / operation between char values");
      }

      case Value::Type::I8:
      case Value::Type::I16:
      case Value::Type::I32:
      case Value::Type::I64: {
        int64_t result = lhs.AsSInt() / rhs.AsSInt();
        v.size = Value::ResolveSWordSize(result);
        Value::WriteSValue(v , result);
        return v;
      }

      case Value::Type::U8:
      case Value::Type::U16:
      case Value::Type::U32:
      case Value::Type::U64: {
        uint64_t result = lhs.AsUInt() / rhs.AsUInt();
        v.size = Value::ResolveUWordSize(result);
        Value::WriteUValue(v , result);
        return v;
      }

      case Value::Type::F32: {
        float result = lhs.AsFloat() / rhs.AsFloat();
        Value::Write(v , result);
        return v;
      }

      case Value::Type::F64: {
        double result = lhs.AsDouble() / rhs.AsDouble();
        Value::Write(v , result);
        return v;
      }

      default:
        throw RuntimeError("Invalid / operation between values");
    }

    throw RuntimeError("Invalid / operation between values");
  }

} // namespace ylang

