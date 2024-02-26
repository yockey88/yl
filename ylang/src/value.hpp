#ifndef YL_VALUE_HPP
#define YL_VALUE_HPP

#include <cstdint>
#include <variant>

#include "defines.hpp"
#include "token.hpp"

namespace ylang {

  template <typename T> 
  concept value_t = std::is_arithmetic_v<T> || std::is_same_v<T , char> || 
                    std::is_same_v<T , RegisterType> || std::is_same_v<T , address_t> ||
                    std::is_same_v<T , std::monostate>;

  typedef std::variant<
      bool , char ,
      int8_t , int16_t , int32_t , int64_t , 
      uint8_t , uint16_t , uint32_t , uint64_t , 
      float , double , 
      address_t ,
      RegisterType ,
      std::monostate
    > ValueData;

  constexpr static uint32_t kBoolIndex = 0;
  constexpr static uint32_t kCharIndex = 1;
  constexpr static uint32_t kInt8Index = 2;
  constexpr static uint32_t kInt16Index = 3;
  constexpr static uint32_t kInt32Index = 4;
  constexpr static uint32_t kInt64Index = 5;
  constexpr static uint32_t kUInt8Index = 6;
  constexpr static uint32_t kUInt16Index = 7;
  constexpr static uint32_t kUInt32Index = 8;
  constexpr static uint32_t kUInt64Index = 9;
  constexpr static uint32_t kFloatIndex = 10;
  constexpr static uint32_t kDoubleIndex = 11;
  constexpr static uint32_t kAddressIndex = 12;
  constexpr static uint32_t kRegisterIndex = 13;
  constexpr static uint32_t kMonostateIndex = 14;

  class Value {
    public:
      enum Type {
        BOOL , CHAR ,
        I8 , I16 , I32 , I64 ,
        U8 , U16 , U32 , U64 ,
        F32 , F64 ,
        ADDRESS ,
        REGISTER ,
        NIL
      } type = U64;
      WordSize size = QWORD;
      ValueData value;

      template <value_t T>
      [[nodiscard]] T& As() {
        return std::get<T>(value);
      }

      template <value_t T>
      [[nodiscard]] const T& As() const {
        return std::get<T>(value);
      }

      template <value_t T>
      static void Write(Value& value , const T& val) {
        value.value = val;
      }

      uint8_t* AsBytes();
      int64_t AsSInt() const;
      uint64_t AsUInt() const;
      float AsFloat() const;
      double AsDouble() const;
      address_t AsAddress() const;
      RegisterType AsRegister() const;
      bool AsBool() const;

      Value() = default;
      Value(Value&& rhs);
      Value(const Value& rhs);
      Value& operator=(Value&& rhs);
      Value& operator=(const Value& rhs);

      void Zero();
      void Negate();

      bool IsNull() const;
      bool IsNegative() const;
      bool IsZero() const;
      bool IsInt() const;
      bool IsFloat() const;
      bool IsDouble() const;
      bool IsBool() const;
      bool IsSigned() const;
      bool IsUnsigned() const;
      bool IsAddress() const;
      bool IsRegister() const;
      bool TypesCompatible(const Value& rhs) const;

      static Value CreateValue(const Token& token);
      static Value CreateValue(const address_t& addr);
      static Value CreateValue(RegisterType reg);

      static size_t GetTypeSize(Value::Type type);
      static Value::Type GetCommonType(const Value& lhs , const Value& rhs);
      static Value::Type UnsignedToSigned(Value::Type type);
      static Value::Type SignedToUnsigned(Value::Type type);
  };

  bool operator<(const address_t& lhs , const Value& rhs);
  bool operator>(const address_t& lhs , const Value& rhs);
  bool operator==(const address_t& lhs , const Value& rhs);
  bool operator!=(const address_t& lhs , const Value& rhs);
  bool operator<=(const address_t& lhs , const Value& rhs);
  bool operator>=(const address_t& lhs , const Value& rhs);

  address_t operator+(const address_t& lhs , const Value& rhs);
  address_t operator-(const address_t& lhs , const Value& rhs);

  address_t operator+(const Value& lhs , const address_t& rhs);
  address_t operator-(const Value& lhs , const address_t& rhs);

  bool operator<(const Value& lhs , const Value& rhs);
  bool operator>(const Value& lhs , const Value& rhs);
  bool operator==(const Value& lhs , const Value& rhs);
  bool operator!=(const Value& lhs , const Value& rhs);
  bool operator<=(const Value& lhs , const Value& rhs);
  bool operator>=(const Value& lhs , const Value& rhs);

  Value operator+(const Value& lhs , const Value& rhs);
  Value operator-(const Value& lhs , const Value& rhs);
  Value operator*(const Value& lhs , const Value& rhs);
  Value operator/(const Value& lhs , const Value& rhs);

  constexpr static uint32_t kNumPrimitiveTypes = Value::Type::NIL + 1;
  constexpr std::array<std::string_view , kNumPrimitiveTypes> kValueStrings = {
    "bool" , "char" ,
    "int8" , "int16" , "int32" , "int64" , 
    "uint8" , "uint16" , "uint32" , "uint64" , 
    "float" , "double" , 
    "address" , "register" , "nil"
  };

} // namespace ylang

template<>
struct fmt::formatter<ylang::Value> : fmt::formatter<std::string_view> {
  auto format(ylang::Value val , fmt::format_context& ctx) const {
    std::string type = "UNKNOWN";
    std::string size = "UNKNOWN";
    std::string data = "UNKNOWN";

    auto index = val.value.index();

    switch (val.type) {
      case ylang::Value::ADDRESS:
        type = "ADDRESS";
        size = std::to_string(val.size);
      break;
      case ylang::Value::REGISTER:
        type = "REGISTER";
        size = std::to_string(val.size);
      break;
      case ylang::Value::I8:
        type = "I8";
        size = std::to_string(val.size);
      break;
      case ylang::Value::I16:
        type = "I16";
        size = std::to_string(val.size);
      break;
      case ylang::Value::I32:
        type = "I32";
        size = std::to_string(val.size);
      break;
      case ylang::Value::I64:
        type = "I64";
        size = std::to_string(val.size);
      break;
      case ylang::Value::U8:
        type = "U8";
        size = std::to_string(val.size);
      break;
      case ylang::Value::U16:
        type = "U16";
        size = std::to_string(val.size);
      break;
      case ylang::Value::U32:
        type = "U32";
        size = std::to_string(val.size);
      break;
      case ylang::Value::U64:
        type = "U64";
        size = std::to_string(val.size);
      break;
      case ylang::Value::F32:
        type = "F32";
        size = std::to_string(val.size);
      break;
      case ylang::Value::F64:
        type = "F64";
        size = std::to_string(val.size);
      break;
      default:
        size = std::to_string(val.size);
    }
    
    switch (index) {
      case ylang::kCharIndex:
        data = std::to_string(val.As<char>());
      break;
      case ylang::kInt8Index:
        data = std::to_string(val.As<int8_t>());
      break;
      case ylang::kInt16Index:
        data = std::to_string(val.As<int16_t>());
      break;
      case ylang::kInt32Index:
        data = std::to_string(val.As<int32_t>());
      break;
      case ylang::kInt64Index:
        data = std::to_string(val.As<int64_t>());
      break;
      case ylang::kUInt8Index:
        data = std::to_string(val.As<uint8_t>());
      break;
      case ylang::kUInt16Index:
        data = std::to_string(val.As<uint16_t>());
      break;
      case ylang::kUInt32Index:
        data = std::to_string(val.As<uint32_t>());
      break;
      case ylang::kUInt64Index:
        data = std::to_string(val.As<uint64_t>());
      break;
      case ylang::kFloatIndex:
        data = std::to_string(val.As<float>());
      break;
      case ylang::kDoubleIndex:
        data = std::to_string(val.As<double>());
      break;
      case ylang::kRegisterIndex:
        data = std::to_string(val.As<ylang::RegisterType>());
      break;
      case ylang::kAddressIndex:
        data = fmt::format(fmt::runtime("{:#08x}") , val.As<ylang::address_t>().address);
      break;
      case ylang::kMonostateIndex:
        data = "MONOSTATE";
      break;
      default:
        data = "UNKNOWN";
    }

    std::string out_str = fmt::format(fmt::runtime("Value<Type: {} Size: {} Data: {}>"), type , size , data);
    return fmt::formatter<std::string_view>::format(out_str , ctx);
  }
};

#endif // !YL_VALUE_HPP
