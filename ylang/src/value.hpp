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

#pragma pack(push , 1)

  class Value {
    public:
      enum Type {
        BOOL , CHAR ,
        I8 , I16 , I32 , I64 ,
        U8 , U16 , U32 , U64 ,
        F32 , F64 ,
        ADDRESS ,
        REGISTER ,
        STRING ,
        ARRAY ,
        STRUCT ,
        NIL , VOID ,
        CALLABLE ,

        COUNT
      } type = NIL;
      WordSize size = QWORD;
      ValueData value = std::monostate{};

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

      static void WriteSValue(Value& value , const int64_t& val);
      static void WriteUValue(Value& value , const uint64_t& val);

      uint8_t* AsBytes();
      int64_t AsSInt() const;
      uint64_t AsUInt() const;
      float AsFloat() const;
      double AsDouble() const;
      address_t AsAddress() const;
      RegisterType AsRegister() const;
      bool AsBool() const;
      char AsChar() const;
      std::string AsString() const;

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
      bool IsChar() const;
      bool IsSigned() const;
      bool IsUnsigned() const;
      bool IsAddress() const;
      bool IsRegister() const;
      bool IsString() const;
      bool IsArray() const;
      bool TypesCompatible(const Value& rhs) const;

      static bool TypesCompatible(Value::Type lhs , Value::Type rhs);
      static Value::Type GetType(const Token& token);
      static TokenType GetTokenType(Value::Type type);
      static Value CreateValue(const Token& token);
      static Value CreateValue(bool val);
      static Value CreateValue(char val);
      static Value CreateValue(const address_t& addr);
      static Value CreateValue(RegisterType reg);

      static size_t GetTypeSize(Value::Type type);
      static Value::Type GetCommonType(Value::Type lhs , Value::Type rhs);
      static Value::Type GetCommonType(const Value& lhs , const Value& rhs);
      static WordSize ResolveSWordSize(int64_t size);
      static WordSize ResolveUWordSize(uint64_t size);
      static Value::Type UnsignedToSigned(Value::Type type);
      static Value::Type SignedToUnsigned(Value::Type type);
  };

#pragma pack(pop)

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
  bool operator&&(const Value& lhs , const Value& rhs);
  bool operator||(const Value& lhs , const Value& rhs);

  Value operator+(const Value& lhs , const Value& rhs);
  Value operator-(const Value& lhs , const Value& rhs);
  Value operator*(const Value& lhs , const Value& rhs);
  Value operator/(const Value& lhs , const Value& rhs);

  constexpr static uint32_t kNumPrimitiveTypes = Value::Type::COUNT;
  constexpr std::array<std::string_view , kNumPrimitiveTypes> kValueStrings = {
    "bool" , "char" ,
    "int8" , "int16" , "int32" , "int64" , 
    "uint8" , "uint16" , "uint32" , "uint64" , 
    "float" , "double" , 
    "address" , "register" , "string" , "array" ,
    "struct" , 
    "nil" , "void" ,
    "callable"
  };

} // namespace ylang

template <>
struct fmt::formatter<ylang::Value::Type> : fmt::formatter<std::string_view> {
  auto format(ylang::Value::Type type , fmt::format_context& ctx) const {
    std::string type_str = "UNKNOWN";

    switch (type) {
      case ylang::Value::Type::BOOL:
        type_str = "BOOL";
      break;
      case ylang::Value::Type::CHAR:
        type_str = "CHAR";
      break;
      case ylang::Value::Type::I8:
        type_str = "I8";
      break;
      case ylang::Value::Type::I16:
        type_str = "I16";
      break;
      case ylang::Value::Type::I32:
        type_str = "I32";
      break;
      case ylang::Value::Type::I64:
        type_str = "I64";
      break;
      case ylang::Value::Type::U8:
        type_str = "U8";
      break;
      case ylang::Value::Type::U16:
        type_str = "U16";
      break;
      case ylang::Value::Type::U32:
        type_str = "U32";
      break;
      case ylang::Value::Type::U64:
        type_str = "U64";
      break;
      case ylang::Value::Type::F32:
        type_str = "F32";
      break;
      case ylang::Value::Type::F64:
        type_str = "F64";
      break;
      case ylang::Value::Type::STRING:
        type_str = "STRING";
      break;
      case ylang::Value::Type::ARRAY:
        type_str = "ARRAY";
      break;
      case ylang::Value::Type::ADDRESS:
        type_str = "ADDRESS";
      break;
      case ylang::Value::Type::REGISTER:
        type_str = "REGISTER";
      break;
      case ylang::Value::Type::STRUCT:
        type_str = "STRUCT";
      break;
      case ylang::Value::Type::NIL:
        type_str = "NIL";
      break;
      default:
        type_str = "UNKNOWN";
    }

    return fmt::formatter<std::string_view>::format(type_str , ctx);
  }

};

template <>
struct fmt::formatter<ylang::WordSize> : fmt::formatter<std::string_view> {
  auto format(ylang::WordSize size , fmt::format_context& ctx) const {
    std::string size_str = "UNKNOWN";

    switch (size) {
      case ylang::WordSize::BYTE:
        size_str = "BYTE";
      break;
      case ylang::WordSize::WORD:
        size_str = "WORD";
      break;
      case ylang::WordSize::DWORD:
        size_str = "DWORD";
      break;
      case ylang::WordSize::QWORD:
        size_str = "QWORD";
      break;
      default:
        size_str = fmt::format(fmt::runtime("{}") , static_cast<size_t>(size));;
    }

    return fmt::formatter<std::string_view>::format(size_str , ctx);
  }
};

template <>
struct fmt::formatter<ylang::ValueData> : fmt::formatter<std::string_view> {
  auto format(ylang::ValueData val , fmt::format_context& ctx) const {
    std::string data = "UNKNOWN";

    std::visit([&data](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T , bool>) {
        data = arg ? "true" : "false";
      } else if constexpr (std::is_same_v<T , char>) {
        data.clear();
        data += arg;
      } else if constexpr (std::is_same_v<T , uint8_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , uint16_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , uint32_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , uint64_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , int8_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , int16_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , int32_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , int64_t>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , float>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , double>) {
        data = std::to_string(arg);
      } else if constexpr (std::is_same_v<T , ylang::RegisterType>) {
        data = ylang::RegisterStrings[static_cast<size_t>(arg)];
      } else if constexpr (std::is_same_v<T , ylang::address_t>) {
        data = fmt::format(fmt::runtime("{:#08x}") , arg.address);
      } else if constexpr (std::is_same_v<T , std::monostate>) {
        data = "nil";
      } else {
        data = "UNKNOWN";
      }
    } , val);

    return fmt::formatter<std::string_view>::format(data , ctx);
  }
};

template<>
struct fmt::formatter<ylang::Value> : fmt::formatter<std::string_view> {
  auto format(ylang::Value val , fmt::format_context& ctx) const {
    std::string type = fmt::format(fmt::runtime("Type: {}") , val.type);
    std::string size = fmt::format(fmt::runtime("Size: {}") , val.size);
    std::string data = fmt::format(fmt::runtime("Data: {}") , val.value);    

    std::string out_str = fmt::format(fmt::runtime("Value<{} {} {}>"), type , size , data);
    return fmt::formatter<std::string_view>::format(out_str , ctx);
  }
};

#endif // !YL_VALUE_HPP
