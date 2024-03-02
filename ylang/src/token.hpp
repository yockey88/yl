#ifndef YL_TOKEN_HPP
#define YL_TOKEN_HPP

#include <string>

#include "defines.hpp"

namespace ylang {

  struct Token {
    Token() = default;
    constexpr Token(SourceLocation loc , TokenType type, const std::string& value) 
      : loc(loc) , type(type), value(value) {}

    Token(Token&& other) 
      : loc(other.loc) , type(other.type), value(std::move(other.value)) {}
    Token(const Token& other) 
      : loc(other.loc) , type(other.type), value(other.value) {}
    Token& operator=(Token&& other) {
      loc = other.loc;
      type = other.type;
      value = std::move(other.value);
      return *this;
    }

    Token& operator=(const Token& other) {
      loc = other.loc;
      type = other.type;
      value = other.value;
      return *this;
    }

    SourceLocation loc;
    TokenType type;
    std::string value;
  };

  static size_t GetTypeSize(TokenType type) {
    switch (type) {
      case TokenType::I8: 
      case TokenType::U8: return BYTE;

      case TokenType::I16:
      case TokenType::U16: return WORD;

      case TokenType::I32:
      case TokenType::U32:
      case TokenType::F32: return DWORD;

      case TokenType::I64:
      case TokenType::U64:
      case TokenType::F64:
      case TokenType::ADDRESS: return QWORD;
      
      default: return 0;
    }
  } 

} // namespace ylang

#endif // !YL_TOKEN_HPP
