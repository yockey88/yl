#ifndef ylang_lexer_hpp
#define ylang_lexer_hpp

#include <string>
#include <vector>
#include <unordered_map>

#include "defines.hpp"
#include "token.hpp"
#include "errors.hpp"
#include "preprocessor/preprocessor.hpp"

namespace ylang {

  struct TokenList {
    std::string src_name;
    std::vector<Token> tokens{};
    std::vector<std::string> dependency_names{};  
    bool error = true;

    bool valid() const { return !error; }
    size_t size() const { return tokens.size(); }
  };

  class Lexer {
    public:
      Lexer(const ProcessedFile& src);
      ~Lexer() {}

      TokenList Lex();

    private:
      SourceLocation loc = {1, 1};

      TokenList tokens;

      const std::string src_name;
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

      void HandleString(char delim);

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
