#include "parser/parser.hpp"

#include "defines.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {

  Ast Parser::Parse() {
    if (tokens.size() <= 2) {
      return ast;
    }

    Consume(TokenType::START , "Expected start of file");

    while (!AtEnd()) {
      if (Check(TokenType::END)) {
        break;
      }

      Stmt* stmt = ParseDeclaration();
      if (stmt != nullptr) {
        ast.nodes.push_back(stmt);
      } else {
        // Sync();
        return ast;
      }
    }

    Consume(TokenType::END , "Expected end of file");

    ast.Validate();
    return ast;
  }

  Stmt* Parser::ParseDeclaration() {
    try {
      if (Check(TokenType::IDENTIFIER)) {
        return ResolveDeclaration();  
      }
      return ParseStatement();
    } catch (const ParserError& e) {
      printerr(ErrorType::PARSER , "Failed to parse declaration");
      printerr(ErrorType::PARSER , e.what());
      return nullptr;
    }
  }

  Stmt* Parser::ResolveDeclaration() {
    Token name = Consume(TokenType::IDENTIFIER , "Expected identifier");

    if (Match({ TokenType::EQUAL , TokenType::COLON , TokenType::SEMICOLON })) {
      if (Previous().type == TokenType::SEMICOLON) {
        return new VarDeclStmt(name , nullptr);
      }

      if (Previous().type == TokenType::COLON) {
        Token type = ConsumeType(fmtstr("Expected type after ':' found '{}'" , TokenTypeStrings[Peek().type]));
        Consume(TokenType::EQUAL , "Expected '=' after type declaration");
        return ParseVarDeclaration(name , type);
      }

      return ParseVarDeclaration(name);
    }

    throw Error("Expected '=', ':', or ';'");
  }

  Stmt* Parser::ParseVarDeclaration(const Token& name , const Token& type) {
    Expr* initializer = ParseExpression();
    Consume(TokenType::SEMICOLON , "Expected semicolon");

    if (type.type == TokenType::UNKNOWN) {
      return new VarDeclStmt(name , initializer);
    }

    return new VarDeclStmt(name , initializer , type);
  }

  Stmt* Parser::ParseStatement() {
    if (Match({ TokenType::OPEN_BRACE })) {
      return ParseBlock();
    }
    return new ExprStmt(ParseExpression());
  }

  Stmt* Parser::ParseBlock() {
    std::vector<Stmt*> stmts;

    while (!Check(TokenType::CLOSE_BRACE) && !AtEnd()) {
      try {
        Stmt* stmt = ParseDeclaration();
        if (stmt != nullptr) {
          stmts.push_back(stmt);
        } else {
          Sync();
        }
      } catch (const ParserError& e) {
        printerr(ErrorType::PARSER , e.what());
        return nullptr;
      }
    }

    Consume(TokenType::CLOSE_BRACE , "Expected closing brace");

    return new BlockStmt(stmts);
  }

  Expr* Parser::ParseExpression() {
    return ParseTerm();
  }

  Expr* Parser::ParseTerm() {
    Expr* expr = ParseFactor();

    while (Match({ TokenType::PLUS , TokenType::MINUS })) {
      Token op = Previous();
      Expr* right = ParseFactor();
      expr = new BinaryExpr(expr , op , right);
    }

    return expr;
  }

  Expr* Parser::ParseFactor() {
    Expr* expr = ParseLiteral();
    
    while (Match({ TokenType::STAR , TokenType::F_SLASH })) {
      Token op = Previous();
      Expr* right = ParseLiteral();
      expr = new BinaryExpr(expr , op , right);
    }


    return expr;
  }
  
  Expr* Parser::ParseLiteral() {
    if (MatchLiterals()) {
      return new LiteralExpr(Previous());
    }
    
    return ResolveExpr();
  }
      
  Expr* Parser::ResolveExpr() {
    if (Match({ TokenType::OPEN_PAREN })) {
      Expr* expr = ParseExpression();
      Consume(TokenType::CLOSE_PAREN , "Expected closing parenthesis");
      return new GroupingExpr(expr);
    }

    if (Match({ TokenType::PLUS , TokenType::MINUS })) {
      Token op = Previous();
      Expr* right = ParseLiteral();
      return new UnaryExpr(op , right);
    }

    if (Match({ TokenType::IDENTIFIER })) {
      return new VarExpr(Previous());
    }

    throw Error(fmtstr("Failed to resolve expression at '{}'" , Peek().value));
  }

  Token Parser::Peek() const {
    if (AtEnd()) {
      return tokens.back();
    }

    return tokens[current];
  }

  Token Parser::Previous() const {
    if (current == 0) {
      return tokens.front();
    }

    return tokens[current - 1];
  }

  Token Parser::Advance() {
    if (!AtEnd()) {
      current++;
    }

    return Previous();
  }
      
  Token Parser::ConsumeType(const std::string& message) {
    if (AtEnd()) {
      throw Error(message);
    }

    switch (Peek().type) {
      case TokenType::BOOL:
      case TokenType::CHAR:
      case TokenType::STRING:
      case TokenType::I8:
      case TokenType::I16:
      case TokenType::I32:
      case TokenType::I64:
      case TokenType::U8:
      case TokenType::U16:
      case TokenType::U32:
      case TokenType::U64:
      case TokenType::F32:
      case TokenType::F64:
        return Advance();
      default:
        throw Error(message);
    }

    return Advance();
  }
 
  Token Parser::Consume(TokenType type, const std::string& message) {
    if (Check(type)) {
      return Advance();
    }

    throw Error(message);
  }
  
  bool Parser::AtEnd() const {
    return current >= tokens.size();
  }

  bool Parser::Check(TokenType type) const {
    if (AtEnd()) {
      return false;
    }

    return Peek().type == type;
  }
  
  bool Parser::Match(const std::vector<TokenType>& types , bool advance) {
    for (const auto& type : types) {
      if (Check(type)) {
        if (advance) {
          Advance();
        }

        return true;
      }
    }

    return false;
  }
      
  bool Parser::MatchLiterals(bool advance) {
    return Match({ TokenType::SINTEGER , TokenType::INTEGER , 
                TokenType::FLOAT , TokenType::DOUBLE } , advance);
  }

  bool Parser::MatchTypes(bool advance) {
    return Match({ TokenType::I8 , TokenType::I16 , TokenType::I32 , TokenType::I64 , 
                TokenType::U8 , TokenType::U16 , TokenType::U32 , TokenType::U64 , 
                TokenType::F32 , TokenType::F64 } , advance);
  }
  
  void Parser::Sync() {
    Advance();

    while (!AtEnd()) {
      // if (Previous().type == TokenType::SEMICOLON) {
      //   return;
      // }

      switch (Peek().type) {
        default:
          Advance();
      }
    }
  }
  
  ParserError Parser::Error(const std::string& message) const {
    SourceLocation loc = Peek().loc; 
    loc.index = current;
    return ParserError(loc , message);
  }

} // namespace ylang
