#ifndef ylang_lexer_hpp
#define ylang_lexer_hpp

#include <string>
#include <vector>
#include <unordered_map>

#include "defines.hpp"
#include "token.hpp"
#include "errors.hpp"

namespace ylang {

  // static constexpr uint32_t kFnvOffsetBasisU32 = 0x811C9DC5;
  // static constexpr uint32_t kFnvPrimeU32 = 0x01000193;

  static constexpr uint64_t kFnvOffsetBasis = 0xBCF29CE484222325;
  static constexpr uint64_t kFnvPrime = 0x100000001B3;

  static constexpr uint64_t FNV(std::string_view str) {
    uint64_t hash = kFnvOffsetBasis;
    for (auto& c : str) {
      hash ^= c;
      hash *= kFnvPrime;
    }
    hash ^= str.length();
    hash *= kFnvPrime;

    return hash;
  }

  struct TokenList {
    std::vector<Token> tokens{};
    bool error = true;

    bool valid() const { return !error; }
    size_t size() const { return tokens.size(); }
  };

  class Lexer {
    public:
      Lexer(const std::string& src);
      ~Lexer() {}

      TokenList Lex();

    private:
      SourceLocation loc = {1, 1};

      TokenList tokens;

      const std::string src;
      std::string current_token;

      struct Flags {
        bool sign = false;
      } flags;

      static std::unordered_map<uint64_t, TokenType> keywords;

      void HandleWhitespace();

      void HandleNumeric();
      void HandleFloat();
      void HandleScientific();
      void HandleHex();

      void HandleAlpha();
      void HandleAlphaNumeric();

      void HandleOperator();
      void HandleComment();

      void AddToken(TokenType type);

      void NewLine(bool advance = true);
      void Consume();
      void Advance();
      void DiscardToken();

      bool AtEnd() const;
      char Peek() const;
      char PeekNext() const;
      bool Check(char c) const;
      bool CheckNext(char c) const;
      bool Match(char expected);
      TokenType GetKeywordType(const std::string& str) const;

      bool IsNumeric(char c) const;
      bool IsAlpha(char c) const;
      bool IsAlphaNumeric(char c) const;
      bool IsWhitespace(char c) const;
      bool IsKeyword(const std::string& str) const;
      bool IsOperator(char c) const;


      template <typename... Args>
      LexerError Error(std::string_view message, Args&&... args) {
        return LexerError(loc, fmt::format(fmt::runtime(message) , std::forward<Args>(args)...));
      }
  };

} // namespace ylang

#endif // !ylang_lexer_hpp
