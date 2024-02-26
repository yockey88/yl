#ifndef YL_TOKEN_HPP
#define YL_TOKEN_HPP

#include <string>

#include "defines.hpp"

namespace ylang {

  struct Token {
    constexpr Token(SourceLocation loc , TokenType type, const std::string& value) 
      : loc(loc) , type(type), value(value) {}

    SourceLocation loc;
    const TokenType type;
    const std::string value;
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
