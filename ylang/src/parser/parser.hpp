#ifndef YL_PARSER_HPP
#define YL_PARSER_HPP

#include <string>

#include "token.hpp"
#include "errors.hpp"
#include "lexer/lexer.hpp"
#include "ast/ast.hpp"

template <>
struct std::hash<ylang::Token> {
  std::size_t operator()(const ylang::Token& token) const {
    return std::hash<std::string>{}(token.value);
  }
};

namespace ylang {

  enum CallableType {
    FUNCTION,

    NUM_CALLABLE_TYPES
  };


  class Parser {
    public:
      Parser(const TokenList& tokens) 
        : tokens(tokens) {}
      ~Parser() = default;

      Ast Parse();

    private:
      Ast ast;
      TokenList tokens;

      size_t current = 0;

      std::vector<Token> declared_types;

      Stmt* ParseDeclaration();
      Stmt* ResolveDeclaration();

      Stmt* ParseCallable(CallableType type , const Token& name , 
                          const Token& ret_type = Token(SourceLocation{} , TokenType::UNKNOWN , ""));
      Stmt* ParseFunctionDeclaration(const Token& name , const Token& ret_type = Token(SourceLocation{} , TokenType::UNKNOWN , ""));
      Stmt* ParseVarDeclaration(const Token& name , 
          const Token& type = Token(SourceLocation{} , TokenType::UNKNOWN , ""));
      Stmt* ParseArrayDeclaration(const Token& name , 
          const Token& type = Token(SourceLocation{} , TokenType::UNKNOWN , ""));

      Stmt* ParseStructDeclaration();
                                                                                                   
      Stmt* ParseStatement();                                                                      
      Stmt* ParseBlock();                                                                          
      /// DEBUG only, will replace later 
      Stmt* ParsePrint();
      Stmt* ParseIf();
      Stmt* ParseWhile();
      Stmt* ParseFor();
      Stmt* ParseReturn();
                                                                                                   
      Expr* ParseExpression();                                                                     
      Expr* ParseAssignment();
      Expr* ParseOr();
      Expr* ParseAnd();
      Expr* ParseEquality();
      Expr* ParseComparison();
      Expr* ParseTerm();
      Expr* ParseFactor();
      Expr* ParseCall();
      Expr* FinishCall(Expr* callee);
      Expr* ParseLiteral();
      Expr* ResolveExpr();

      Expr* ParseArrayExpr(Token size);
      Expr* ParseArrayAccess(Expr* expr);
      Expr* ParseStructAccess(Expr* expr);

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
      bool DeclaredType(const Token& type) const;
      
      void Sync();
      Stmt* RecoverFrom(TokenType type , std::vector<Stmt*>& stmts);

      ParserError Error(const std::string& message) const;
  };

}

#endif // !YL_PARSER_HPP
