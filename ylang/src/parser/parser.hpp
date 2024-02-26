#ifndef YL_PARSER_HPP
#define YL_PARSER_HPP

#include <string>

#include "token.hpp"
#include "errors.hpp"
#include "ast/ast.hpp"

template <>
struct std::hash<ylang::Token> {
  std::size_t operator()(const ylang::Token& token) const {
    return std::hash<std::string>{}(token.value);
  }
};

namespace ylang {


  class Parser {
    public:
      Parser(const std::vector<Token>& tokens) 
        : tokens(tokens) {}
      ~Parser() = default;

      Ast Parse();

    private:
      Ast ast;
      std::vector<Token> tokens;

      size_t current = 0;

      Stmt* ParseDeclaration();
      Stmt* ResolveDeclaration();
      Stmt* ParseVarDeclaration(const Token& name , 
          const Token& type = Token(SourceLocation{} , TokenType::UNKNOWN , ""));
                                                                                                   
      Stmt* ParseStatement();                                                                      
                                                                                                   
      Stmt* ParseBlock();                                                                          
                                                                                                   
      Expr* ParseExpression();                                                                     
                                                                                                   
      /// parses addition and subctraction                                                         
      ///   binary expressions                                                                     
      Expr* ParseTerm();
      /// parses multiplication and division
      ///  binary expressions
      Expr* ParseFactor();
      Expr* ParseLiteral();
      Expr* ResolveExpr();

      Token Peek() const;
      Token Previous() const;
      Token Advance();
      Token Consume(TokenType type, const std::string& message);
      Token ConsumeType(const std::string& message);

      bool AtEnd() const;
      bool Check(TokenType type) const;
      bool Match(const std::vector<TokenType>& types , bool advance = true);
      bool MatchLiterals(bool advance = true);
      bool MatchTypes(bool advance = true);

      void Sync();

      ParserError Error(const std::string& message) const;
  };

}

#endif // !YL_PARSER_HPP
