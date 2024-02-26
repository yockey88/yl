#include "lexer/lexer.hpp"

#include <cmath>
#include <sstream>
#include <iostream>

#include "util/io.hpp"

namespace ylang {

  std::unordered_map<uint64_t, TokenType> Lexer::keywords{};

  Lexer::Lexer(const std::string& src) 
      : src(src) {
    keywords[FNV("bool")] = TokenType::BOOL;
    keywords[FNV("char")] = TokenType::CHAR;
    keywords[FNV("string")] = TokenType::STRING;
    keywords[FNV("u8")] = TokenType::U8;
    keywords[FNV("u16")] = TokenType::U16;
    keywords[FNV("u32")] = TokenType::U32;
    keywords[FNV("u64")] = TokenType::U64;
    keywords[FNV("i8")] = TokenType::I8;
    keywords[FNV("i16")] = TokenType::I16;
    keywords[FNV("i32")] = TokenType::I32;
    keywords[FNV("i64")] = TokenType::I64;
    keywords[FNV("f32")] = TokenType::F32;
    keywords[FNV("f64")] = TokenType::F64;
    keywords[FNV("true")] = TokenType::TRUE;
    keywords[FNV("false")] = TokenType::FALSE;
    keywords[FNV("return")] = TokenType::RETURN;
  }

  TokenList Lexer::Lex() {
    if (src.empty() || src == "") {
      tokens.error = false;
      return tokens;
    }

    tokens.tokens.push_back(Token(loc , TokenType::START , ""));

    loc.index = 0;
    loc.line = 1;
    loc.column = 1;

    while (!AtEnd()) {
      try {
        char c = Peek();

        if (IsWhitespace(c)) {
          HandleWhitespace();
          continue;
        }

        if (IsNumeric(c)) {
          HandleNumeric();
          continue;
        }

        if (IsAlpha(c)) {
          HandleAlpha();
          continue;
        }

        if (std::find(kOperators.begin() , kOperators.end() , c) != kOperators.end()) {
          HandleOperator();
          continue;
        }

        throw Error("Could not handle character : '{}'" , c);

      } catch (LexerError& e) {
        printerr(ErrorType::LEXER , e.what());
        return tokens; 
      }
    }

    tokens.tokens.push_back(Token(loc , TokenType::END , ""));
    tokens.error = false;
    return tokens;
  }

  void Lexer::HandleWhitespace() {
    while (IsWhitespace(Peek())) {
      if (Check('\n')) {
        NewLine();
      } else {
        Consume();
      }
    }
  }

  void Lexer::HandleNumeric() {
    while (IsNumeric(Peek())) {
      Advance();

      if (AtEnd()) {
        break;
      }
    }

    if (Check('.')) { 
      HandleFloat();
    } else if (Check('e') || Check('E')) { 
      HandleScientific();
    } else if (Check('x') || Check('X')) {
      if (current_token != "0") {
        throw Error("Hexadecimal number must start with '0x' or '0X'");
      }

      HandleHex();
    } else {
      AddToken(TokenType::INTEGER);
    }
    flags.sign = false;
  }

  void Lexer::HandleFloat() {
    Advance(); // Consume '.'
    if (IsNumeric(Peek())) {
      while (IsNumeric(Peek())) {
        Advance();
      }
    } 

    if (Check('e') || Check('E')) {
      HandleScientific();
      return;
    }

    if (Check('f')) {
      Consume();
    }

    if (std::stod(current_token) > std::numeric_limits<float>::max()) {
      AddToken(TokenType::DOUBLE);
    } else {
      AddToken(TokenType::FLOAT);
    }
  }

  void Lexer::HandleScientific() {
    Consume(); // Consume 'e' or 'E'

    bool small = false;

    double lead_digit;
    try {
      lead_digit = std::stod(current_token);
    } catch (std::out_of_range& e) {
      throw Error("Scientific notation lead digit out of range");
    } catch (std::invalid_argument& e) {
      throw Error("Scientific notation lead digit is not a number");
    }

    DiscardToken();

    if (Check('-')) {
      Consume();
      small = true;
    }

    if (IsNumeric(Peek())) {
      while (IsNumeric(Peek())) {
        Advance();
      }
    }

    uint32_t val;
    try {
      val = std::stoi(current_token);
    } catch (std::out_of_range& e) {
      throw Error("Scientific notation exponent out of range");
    } catch (std::invalid_argument& e) {
      throw Error("Scientific notation exponent is not a number");
    } 

    std::string small_str = "0.";
    if (small) {
      auto pos = std::to_string(lead_digit).find('.');
      std::string lead_digit_str = pos == std::string::npos ? 
        std::to_string(lead_digit) : 
        std::to_string(lead_digit).substr(0, pos) + std::to_string(lead_digit).substr(pos + 1, current_token.size() - pos - 1);

      while (lead_digit_str[lead_digit_str.size() - 1] == '0') {
        lead_digit_str.pop_back();
      }

      for (uint32_t i = 0; i < val - 1; i++) {
        small_str += "0";
      }
      small_str += lead_digit_str;
    }

    std::string  result = small ? 
      small_str :
      std::to_string(lead_digit * std::pow(10, val));
    current_token = result;

    if (std::stod(current_token) > std::numeric_limits<float>::max()) {
      AddToken(TokenType::DOUBLE);
    } else {
      AddToken(TokenType::FLOAT);
    }
  }

  void Lexer::HandleHex() {
    DiscardToken(); // Discard '0'
    Consume(); // Consume 'x' or 'X'
    
    if (!IsNumeric(Peek()) && !(Peek() >= 'a' && Peek() <= 'f') && !(Peek() >= 'A' && Peek() <= 'F')) {
      throw Error("Hexadecimal number must contain at least one digit");
    }

    while (IsNumeric(Peek()) || (Peek() >= 'a' && Peek() <= 'f') || (Peek() >= 'A' && Peek() <= 'F')) {
      Advance();
    }

    uint64_t val;
    std::stringstream ss;
    ss << std::hex << current_token;
    ss >> val;

    current_token = std::to_string(val);

    if (flags.sign) {
      AddToken(TokenType::SINTEGER);
    } else {
      AddToken(TokenType::INTEGER);
    }
  }

  void Lexer::HandleAlpha() {
    while (IsAlphaNumeric(Peek()) || Peek() == '_') {
      Advance();
    }

    if (IsKeyword(current_token)) {
      AddToken(GetKeywordType(current_token));
    } else {
      AddToken(TokenType::IDENTIFIER);
    }
  }
  
  void Lexer::HandleOperator() {
    char c = Peek();

    Advance();

    switch (c) {
      case '{':
        AddToken(TokenType::OPEN_BRACE);
      break;
      case '}':
        AddToken(TokenType::CLOSE_BRACE);
      break;
      case '(':
        AddToken(TokenType::OPEN_PAREN);
      break;
      case ')':
        AddToken(TokenType::CLOSE_PAREN);
      break;
      case '+': 
        AddToken(TokenType::PLUS);
      break;
      case '-':
        AddToken(TokenType::MINUS);
        if (IsNumeric(Peek())) {
          flags.sign = true;
        }
      break;
      case '*':
        AddToken(TokenType::STAR);
      break;
      case '/':
        if (CheckNext('/') || CheckNext('*')) {
          HandleComment();
        } else {
          AddToken(TokenType::F_SLASH);
        }
      break;
      case '=':
        AddToken(TokenType::EQUAL);
      break;
      case ';':
        AddToken(TokenType::SEMICOLON);
      break;
      case ':':
        AddToken(TokenType::COLON);
      break;

      default:
        throw Error("Unknown operator : '{}'" , c);
    }
  }
      
  void Lexer::HandleComment() {
    SourceLocation start = loc;

    Consume();

    /// TODO: stream comments file/log
    if (Check('/')) {
      while (!Check('\n') && !AtEnd()) {
        Consume();
      }
    } else if (Check('*')) {
      while (!(Check('*') && CheckNext('/')) && !AtEnd()) {
        if (Check('\n')) {
          NewLine();
        }
        Consume();
      }

      if (AtEnd()) {
        throw LexerError(start, "Unterminated block comment");
      }

      Consume();
      Consume();
    } else {
      throw LexerError(start, "Unknown comment type");
    }
  }
     
  void Lexer::AddToken(TokenType type) {
    tokens.tokens.push_back(Token(loc, type, current_token));
    DiscardToken();
  }

  void Lexer::NewLine(bool advance) {
    ++loc.line;
    loc.column = 1;
    if (advance) {
      ++loc.index;
    }
  }

  void Lexer::Consume() {
    ++loc.index;
    ++loc.column;
  }

  void Lexer::Advance() {
    current_token += src[loc.index];
    Consume();
  }

  void Lexer::DiscardToken() {
    current_token = "";
  }

  bool Lexer::AtEnd() const {
    return loc.index >= src.size();
  }

  char Lexer::Peek() const {
    if (AtEnd()) {
      return '\0';
    }
    return src[loc.index];
  }

  char Lexer::PeekNext() const {
    if (AtEnd() || loc.index + 1 >= src.size()) {
      return '\0';
    }
    return src[loc.index + 1];
  }

  bool Lexer::Check(char c) const {
    return Peek() == c;
  }

  bool Lexer::CheckNext(char c) const {
    return PeekNext() == c;
  }

  bool Lexer::Match(char expected) {
    if (!Check(expected)) {
      return false;
    }
    Advance();
    return true;
  }
      
  TokenType Lexer::GetKeywordType(const std::string& str) const {
    try {
      return keywords.at(FNV(str));
    } catch (std::out_of_range& e) {
      throw LexerError(loc , fmtstr("Unknown keyword {}" , str));
    }
  }

  bool Lexer::IsNumeric(char c) const {
    return c >= '0' && c <= '9';
  }

  bool Lexer::IsAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool Lexer::IsAlphaNumeric(char c) const {
    return IsAlpha(c) || IsNumeric(c);
  }

  bool Lexer::IsWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
  }

  bool Lexer::IsKeyword(const std::string& str) const {
    return keywords.find(FNV(str)) != keywords.end();
  }

  bool Lexer::IsOperator(char c) const {
    return std::find(kOperators.begin() , kOperators.end() , c) != kOperators.end();
  }

} // namespace ylang
