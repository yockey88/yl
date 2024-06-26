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

      try {
        Stmt* stmt = ParseDeclaration();
        if (stmt != nullptr) {
          ast.nodes.push_back(stmt);
        } else {
          Sync();
         return ast;
        }
      } catch (const ParserError& e) {
        printfmt("Error in file '{}'" , tokens.src_name);
        printerr(ErrorType::PARSER , "Failed to parse declaration");
        printerr(ErrorType::PARSER , e.what());
        return ast;
      } catch (const InternalError& e) {
        printfmt("Error in file '{}'" , tokens.src_name);
        printerr(ErrorType::INTERNAL , "Internal error");
        printerr(ErrorType::INTERNAL , e.what());
        return ast;
      } catch (const std::exception& e) {
        printfmt("Error in file '{}'" , tokens.src_name);
        printerr(ErrorType::INTERNAL , "Internal error");
        printerr(ErrorType::INTERNAL , e.what());
        return ast;
      }
    }

    Consume(TokenType::END , "Expected end of file");

    ast.Validate();
    
    Stmt* main_ret = nullptr;

    if (ast.nodes.back()->GetType() == NodeType::RETURN_STMT) {
      main_ret = ast.nodes.back();
      ast.nodes.pop_back();
    } else {
      main_ret = new ReturnStmt;
    }

    ast.nodes.push_back(main_ret);

    Stmt* body = new BlockStmt(ast.nodes);

    Token name = Token(SourceLocation{} , TokenType::IDENTIFIER , fmtstr("yl__entry__{}" , tokens.src_name));
    Token ret_val = Token(SourceLocation{} , TokenType::I32 , "__result");
    ast.root = new FunctionStmt(name , std::vector<Token>{} , ret_val , body);

    ast.name = tokens.src_name;
    ast.dependencies = tokens.dependency_names;
    return ast;
  }

  Stmt* Parser::ParseDeclaration() {
    if (Match({ TokenType::IDENTIFIER } , false)) {
      /// variable and function declarations
      return ResolveDeclaration();
    }
    if (Match({ TokenType::OPEN_BRACE } , false)) {
      return ParseBlock();
    }
    if (Match({ TokenType::PRINT })) {
      return ParsePrint();
    }
    if (Match({ TokenType::IF })) {
      return ParseIf();
    }
    if (Match({ TokenType::WHILE })) {
      return ParseWhile();
    }
    if (Match({ TokenType::FOR })) {
      return ParseFor();
    }
    if (Match({ TokenType::DEFN })) {
      return ResolveDeclaration();
    }
    if (Match({ TokenType::RETURN } , false)) {
      return ParseReturn();
    }
    if (Match({ TokenType::STRUCT })) {
      return ParseStructDeclaration();
    }
    return ParseStatement();
  }

  Stmt* Parser::ResolveDeclaration() {
    Token name = Consume(TokenType::IDENTIFIER , "Expected identifier");

    if (DeclaredVar(name)) {
      if (Match({ TokenType::EQUAL })) {
        Expr* assignment = ParseAssignment();
        Consume(TokenType::SEMICOLON , "Expected semicolon after assignment");

        Expr* assign = new AssignExpr(name , assignment);
        return new ExprStmt(assign);
      } else {
        throw Error(fmtstr("Variable '{}' already declared in this scope" , name.value));
      }
    }

    if (Match({ TokenType::SEMICOLON })) {
      declarations[current_scope].vars.push_back(name);
      return new VarDeclStmt(name , nullptr);
    }

    if (DeclaredFunc(name)) {
      if (Match({ TokenType::OPEN_PAREN })) {
        Expr* expr = FinishCall(new VarExpr(name));
        Consume(TokenType::SEMICOLON , "Expected semicolon after function call");
        return new ExprStmt(expr);
      } else {
        throw Error(fmtstr("Unknown use of declared function identifer {}" , name.value));
      }
    }

    if (Match({ TokenType::OPEN_PAREN } , false)) {
      Expr* expr = new VarExpr(name); 

      do {
        if (Match({ TokenType::OPEN_PAREN })) {
          expr = FinishCall(expr);
        } else if (Match({ TokenType::DOT })) {
          Token field = Consume(TokenType::IDENTIFIER , "Expected identifier after '.'");
          expr = new ObjAccessExpr(expr , field);
        } else {
          break;
        }
      } while (true);

      Consume(TokenType::SEMICOLON , "Expected semicolon after function call");

      return new ExprStmt(expr);
    }
    
    if (Match({ TokenType::OPEN_BRACKET })) {
      Expr* array = new VarExpr(name);
      Stmt* access = new ExprStmt(ParseArrayAccess(array));

      Consume(TokenType::SEMICOLON , "Expected semicolon after array access");

      return access;
    }

    if (Match({ TokenType::DOT })) {
      Expr* struct_id = new VarExpr(name);
      return new ExprStmt(ParseStructAccess(struct_id));
    }

    if (Match({ TokenType::EQUAL })) {
      if (Match({ TokenType::OPEN_BRACE , TokenType::OPEN_PAREN } , false)) {
        return ParseCallable(FUNCTION , name);
      }

      Expr* initializer = ParseExpression();
      Consume(TokenType::SEMICOLON , "Expected semicolon after assignment expression");

      declarations[current_scope].vars.push_back(name);
      return new VarDeclStmt(name , initializer);
    }

    if (Match({ TokenType::COLON })) {
      Token type = ConsumeType(fmtstr("Expected type after ':' found '{}' = '{}'" , TokenTypeStrings[Peek().type] , Peek().value));
      
      if (Match({ TokenType::SEMICOLON })) {
        declarations[current_scope].vars.push_back(name);
        return new VarDeclStmt(name , nullptr , type);
      }

      if (DeclaredType(type)) {
        if (Match({ TokenType::OPEN_BRACE })) {
          // Expr* initializer = ParseStructInitializer(name , type);
          throw Error("Struct initializers are not yet implemented");
        }
      }

      if (Match({ TokenType::OPEN_BRACKET })) {
        return ParseArrayDeclaration(name , type);
      }

      if (Match({ TokenType::EQUAL })) {
        return ParseVarDeclaration(name , type);
      }

      throw Error(fmtstr("Expected '=', ':', or '(' after identifier '{}' found {} = {}" , Previous().value , TokenTypeStrings[Peek().type] ,
                          Peek().value));
    }

    throw Error(fmtstr("Expected '=', or ':' after identifier '{}' found {} = {}" , Previous().value , TokenTypeStrings[Peek().type] ,
                        Peek().value));
  }
      
  Stmt* Parser::ParseCallable(CallableType type , const Token& name , const Token& ret_type) {
    switch (type) {
      case FUNCTION:
        return ParseFunctionDeclaration(name , ret_type);
      default:
        throw Error(fmtstr("Unknown or unimplemented callable type named {}" , name.value));
    }
    return nullptr; 
  }
      
  Stmt* Parser::ParseFunctionDeclaration(const Token& name , const Token& ret_type) {
    std::vector<Token> params;
    if (Match({ TokenType::OPEN_PAREN })) {
      while (!Check(TokenType::CLOSE_PAREN) && !AtEnd()) {
        Token param = Consume(TokenType::IDENTIFIER , "Expected identifier for function parameter");
        param.type = TokenType::UNKNOWN;

        if (Match({ TokenType::COLON })) {
          Token type = ConsumeType("Expected type after ':' for function parameter");
          param.type = type.type;
        }

        params.push_back(param);
        if (!Match({ TokenType::COMMA })) {
          break;
        }
      }
      Consume(TokenType::CLOSE_PAREN , "Expected closing parenthesis for function parameters");
    }
    
    Stmt* body = ParseBlock();
    if (body == nullptr) {
      throw Error("Expected function body");
    }

    if (Match({ TokenType::SEMICOLON })) {
      // do nothing
    }

    declarations[current_scope].funcs.push_back(name);
    return new FunctionStmt(name , params , ret_type , body);
  }

  Stmt* Parser::ParseVarDeclaration(const Token& name , const Token& type) {
    if (Match({ TokenType::OPEN_PAREN , TokenType::OPEN_BRACE } , false)) {
      return ParseCallable(FUNCTION , name , type);
    }

    Expr* initializer = ParseExpression();
    Consume(TokenType::SEMICOLON , "Expected semicolon");

    declarations[current_scope].vars.push_back(name);
    if (type.type == TokenType::UNKNOWN) {
      return new VarDeclStmt(name , initializer);
    }

    return new VarDeclStmt(name , initializer , type);
  }
      
  Stmt* Parser::ParseArrayDeclaration(const Token& name , const Token& type) {
    Token size  = Token(Peek().loc , TokenType::INTEGER , "0");
    if (Match({ TokenType::INTEGER })) {
      size = Previous();
    }
    Consume(TokenType::CLOSE_BRACKET , fmtstr("Expected closing bracket after array size , found {} = {}" , TokenTypeStrings[Peek().type] ,
            Peek().value));

    if (Match({ TokenType::SEMICOLON })) {
      return new ArrayDeclStmt(name , nullptr , size , type);
    }

    if (Match({ TokenType::EQUAL })) {
      // do nothing
    }

    Expr* values = ParseArrayExpr(size);

    return new ArrayDeclStmt(name , values , size , type);
  }
      
  Stmt* Parser::ParseStructDeclaration() {
    Token name = Consume(TokenType::IDENTIFIER , "Expected identifier for struct declaration");
    declarations[current_scope].types.push_back(name);

    if (Match({ TokenType::SEMICOLON })) {
      return new StructStmt(name , {});
    }

    std::vector<Stmt*> fields;
    if (Match({ TokenType::OPEN_BRACE })) {
      while (!Check(TokenType::CLOSE_BRACE) && !AtEnd()) {

        Token field = Consume(TokenType::IDENTIFIER , "Expected identifier for struct field");

        Consume(TokenType::COLON , "Expected colon after struct field identifier");

        Token type = ConsumeType("Expected type for struct field");

        if (Match({ TokenType::SEMICOLON })) {
          fields.push_back(new VarDeclStmt(field , nullptr , type));
        } else if (Match({ TokenType::EQUAL })) {
          Expr* initializer = ParseExpression();

          Consume(TokenType::SEMICOLON , "Expected semicolon after struct field declaration");

          fields.push_back(new VarDeclStmt(field , initializer , type));
        } else if (Match({ TokenType::OPEN_BRACKET })) {
          Token size = Token(Peek().loc , TokenType::INTEGER , "0");
          if (Match({ TokenType::INTEGER })) {
            size = Previous();
          }
          Consume(TokenType::CLOSE_BRACKET , fmtstr("Expected closing bracket after array size , found {} = {}" , TokenTypeStrings[Peek().type] ,
                  Peek().value));

          if (Match({ TokenType::SEMICOLON })) {
            fields.push_back(new ArrayDeclStmt(field , nullptr , size , type));
          }

          if (Match({ TokenType::EQUAL })) {
            // do nothing
          }

          Expr* values = ParseArrayExpr(size);

          fields.push_back(new ArrayDeclStmt(field , values , size , type));
        } else {
          throw Error("Expected semicolon or '=' after struct field declaration");
        }
      }

      Consume(TokenType::CLOSE_BRACE , "Expected closing brace for struct declaration");
    }

    if (Match({ TokenType::SEMICOLON })) {
      // do nothing
    }

    return new StructStmt(name , fields);
  }

  Stmt* Parser::ParseStatement() {
    if (Match({ TokenType::OPEN_BRACE })) {
      Stmt* block = ParseBlock();
      if (block == nullptr) {
        throw current;
      }

      return block;
    }
    return new ExprStmt(ParseExpression());
  }

  Stmt* Parser::ParseBlock() {
    Consume(TokenType::OPEN_BRACE , "Expected opening brace for function declaration"); 
    std::vector<Stmt*> stmts;

    current_scope++;

    while (!Check(TokenType::CLOSE_BRACE) && !AtEnd()) {
      Stmt* stmt = ParseDeclaration();
      if (stmt != nullptr) {
        stmts.push_back(stmt);
      } else {
        throw Error("Failed to parse block");
      }
    }

    Consume(TokenType::CLOSE_BRACE , "Expected closing brace");
    if (Match({ TokenType::SEMICOLON })) {
      // do nothing
    }

    declarations[current_scope].types.clear();
    declarations[current_scope].vars.clear();
    declarations[current_scope].funcs.clear();
    current_scope--;

    return new BlockStmt(stmts);
  }
          
  Stmt* Parser::ParsePrint() {
    bool has_paren = false;
    if (Match({ TokenType::OPEN_PAREN })) {
      has_paren = true;
    }

    Expr* expr = ParseExpression();

    if (has_paren) {
      Consume(TokenType::CLOSE_PAREN , "Expected closing parenthesis after print statement");
    }

    Consume(TokenType::SEMICOLON , "Expected semicolon after print statement");

    return new PrintStmt(expr);
  }

  Stmt* Parser::ParseIf() {
    bool has_paren = false;
    if (Match({ TokenType::OPEN_PAREN })) {
      has_paren = true;
    }

    Expr* condition = ParseExpression();

    if (has_paren) {
      Consume(TokenType::CLOSE_PAREN , "Expected closing parenthesis after if condition");
    }

    Stmt* then_branch = ParseDeclaration();

    Stmt* else_branch = nullptr;
    if (Match({ TokenType::ELSE })) {
      else_branch = ParseDeclaration();
    }

    return new IfStmt(condition , then_branch , else_branch);
  }

  Stmt* Parser::ParseWhile() {
    bool has_paren = false;
    if (Match({ TokenType::OPEN_PAREN })) {
      has_paren = true;
    }

    Expr* condition = ParseExpression();

    if (has_paren) {
      Consume(TokenType::CLOSE_PAREN , "Expected closing parenthesis after while condition");
    }

    Stmt* body = ParseDeclaration();

    return new WhileStmt(condition , body);
  }

  Stmt* Parser::ParseFor() {
    Consume(TokenType::OPEN_PAREN , "Expected opening parenthesis after 'for' keyword");

    Stmt* initializer = nullptr;
    if (Match({ TokenType::SEMICOLON })) {
      // do nothing
    } else if (Match({ TokenType::IDENTIFIER } , false)) {
      initializer = ResolveDeclaration();
    } else {
      Expr* expr = ParseExpression();
      initializer = new ExprStmt(expr);
      Consume(TokenType::SEMICOLON , "Expected semicolon after initializer");
    }

    Expr* condition = nullptr;
    if (!Check(TokenType::SEMICOLON)) {
      condition = ParseExpression();
    }
    Consume(TokenType::SEMICOLON , "Expected semicolon after loop condition");

    Expr* increment = nullptr;
    if (!Check(TokenType::CLOSE_PAREN)) {
      increment = ParseExpression();
    }
    Consume(TokenType::CLOSE_PAREN , "Expected closing parenthesis after for loop");

    Stmt* body = ParseDeclaration();

    if (increment != nullptr) {
      body = new BlockStmt({ body , new ExprStmt(increment) });
    }

    if (condition == nullptr) {
      condition = new LiteralExpr({ Peek().loc , TokenType::TRUE , "true" });
    }

    body = new WhileStmt(condition , body);

    if (initializer != nullptr) {
      body = new BlockStmt({ initializer , body });
    }

    return body;
  }
  
  Stmt* Parser::ParseReturn() {
    Consume(TokenType::RETURN , "Expected 'return' keyword");

    if (Match({ TokenType::SEMICOLON })) {
      return new ReturnStmt();
    }

    if (Match({ TokenType::OPEN_BRACE } , false)) {
      Token name = Token(Peek().loc  , TokenType::IDENTIFIER , GenerateClosureName());
      Stmt* stmt = ParseCallable(FUNCTION , name);
      return new ReturnStmt(stmt);
    }

    Expr* expr = ParseExpression();
    Consume(TokenType::SEMICOLON , "Expected semicolon");

    return new ReturnStmt(new ExprStmt(expr));
  }

  Expr* Parser::ParseExpression() {
    return ParseAssignment();
  }
      
  Expr* Parser::ParseAssignment() {
    Expr* expr = ParseOr();

    if (Match({ TokenType::EQUAL })) {
      Token op = Previous();
      Expr* right = ParseAssignment();
      if (expr->GetType() == NodeType::VAR_EXPR) {
        VarExpr* var = reinterpret_cast<VarExpr*>(expr);
        return new AssignExpr(var->name , right);
      }
      throw Error(fmtstr("Invalid assignment target {}" , expr->GetType()));
    }

    return expr;
  }

  Expr* Parser::ParseOr() {
    Expr* expr = ParseAnd();

    while (Match({ TokenType::LOGICAL_OR })) {
      Token op = Previous();
      Expr* right = ParseAnd();
      expr = new BinaryExpr(expr , op , right);
    }

    return expr; 
  }

  Expr* Parser::ParseAnd() {
    Expr* expr = ParseEquality();

    while (Match({ TokenType::LOGICAL_AND })) {
      Token op = Previous();
      Expr* right = ParseEquality();
      expr = new BinaryExpr(expr , op , right);
    }

    return expr;
  }
  
  Expr* Parser::ParseEquality() {
    Expr* expr = ParseComparison();

    while (Match({ TokenType::BANG_EQUAL , TokenType::EQUAL_EQUAL })) {
      Token op = Previous();
      Expr* right = ParseComparison();
      expr = new BinaryExpr(expr , op , right);
    }

    return expr;
  }

  Expr* Parser::ParseComparison() {
    Expr* expr = ParseTerm();

    while (Match({ TokenType::GREATER , TokenType::GREATER_EQUAL , TokenType::LESS , TokenType::LESS_EQUAL })) {
      Token op = Previous();
      Expr* right = ParseTerm();
      expr = new BinaryExpr(expr , op , right);
    }

    return expr;
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
    Expr* expr = ParseCall();
    
    while (Match({ TokenType::STAR , TokenType::F_SLASH })) {
      Token op = Previous();
      Expr* right = ParseCall();
      expr = new BinaryExpr(expr , op , right);
    }


    return expr;
  }
      
  Expr* Parser::ParseCall() {
    Expr* expr = ParseLiteral();

    do {
      if (Match({ TokenType::OPEN_PAREN })) {
        expr = FinishCall(expr);
      } else if (Match({ TokenType::DOT })) {
        Token name = Consume(TokenType::IDENTIFIER , "Expected property name after '.'");
        expr = new ObjAccessExpr(expr , name);
      } else {
        break;
      }
    } while (true);

    return expr;
  }

  Expr* Parser::FinishCall(Expr* callee) {
    std::vector<Expr*> args;

    if (!Check(TokenType::CLOSE_PAREN)) {
      do {
        if (args.size() >= 255) {
          throw Error("Cannot have more than 255 arguments");
        }
        args.push_back(ParseExpression());
      } while (Match({ TokenType::COMMA }));
    }

    Consume(TokenType::CLOSE_PAREN , "Expected closing parenthesis");

    return new CallExpr(callee , args);
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

    if (Match({ TokenType::QUOTE , TokenType::DQUOTE })) {
      return new LiteralExpr(Previous());
    }

    if (Match({ TokenType::IDENTIFIER })) {
      Expr* id = new VarExpr(Previous());

      if (Match({ TokenType::OPEN_BRACKET } , false)) {
        return ParseArrayAccess(id);
      }


      if (Match({ TokenType::DOT })) {
        printfmt("Matched {} | Current {} = {}" , id->ToString() , TokenTypeStrings[Peek().type] , Peek().value);
        return ParseStructAccess(id);
      }

      return new VarExpr(Previous());
    }

    throw Error(fmtstr("Failed to resolve expression at '{}'" , Peek().value));
  }

  Expr* Parser::ParseArrayExpr(Token size) {
    Consume(TokenType::OPEN_BRACE , fmtstr("Expected opening bracket for array literal found {} = {}" , TokenTypeStrings[Peek().type] , 
            Peek().value));

    std::vector<Expr*> elements;
    while (!Match({ TokenType::CLOSE_BRACE })) {
      do {
        if (elements.size() >= kHeapMemorySize) {
          throw Error("Cannot have more than 255 elements in an array");
        }
        elements.push_back(ParseExpression());
      } while (Match({ TokenType::COMMA }));
    }

    Consume(TokenType::SEMICOLON , "Expected closing bracket for array literal");

    if (size.type != TokenType::INTEGER) {
      throw Error(fmtstr("Expected integer literal for array size found {} = {}" , TokenTypeStrings[size.type] , size.value));
    }

    size_t array_size = std::stoul(size.value);
    if (elements.size() > array_size) {
      throw Error(fmtstr("Excess elements in array intitializer {} != {}" , array_size , elements.size()));
    }

    return new ArrayExpr(elements);
  }

  Expr* Parser::ParseArrayAccess(Expr* expr) {
    Consume(TokenType::OPEN_BRACKET , "Expected opening bracket for array access");

    Expr* index = ParseExpression();
    Consume(TokenType::CLOSE_BRACKET , "Expected closing bracket for array access");

    return new ArrayAccessExpr(expr , index);
  }
      
  Expr* Parser::ParseStructAccess(Expr* expr) {
    Token name = Consume(TokenType::IDENTIFIER , "Expected identifier for struct access");

    Expr* assignment = nullptr;
    Expr* index = nullptr;
    
    if (Match({ TokenType::EQUAL })) {
      assignment = ParseExpression();
      Consume(TokenType::SEMICOLON , "Expected semicolon after assignment expression");
    }

    if (Match({ TokenType::OPEN_BRACKET })) {
      index = ParseExpression();
      Consume(TokenType::CLOSE_BRACKET , "Expected closing bracket for struct array access");

      assignment = nullptr;
      if (Match({ TokenType::EQUAL })) {
        assignment = ParseExpression();
        Consume(TokenType::SEMICOLON , "Expected semicolon after assignment expression");
      }
    }

    return new ObjAccessExpr(expr , name , index , assignment);
  }

  Token Parser::Peek() const {
    if (AtEnd()) {
      return tokens.tokens.back();
    }

    return tokens.tokens[current];
  }

  Token Parser::Previous() const {
    if (current == 0) {
      return tokens.tokens.front();
    }

    return tokens.tokens[current - 1];
  }

  Token Parser::Advance() {
    if (!AtEnd()) {
      current++;
    }

    return Previous();
  }
      
  Token Parser::ConsumeType(const std::string& message) {
    if (AtEnd()) {
      throw Error(fmtstr("Consuming type at the end of file : {}" , message));
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
        if (DeclaredType(Peek()) || Peek().type == TokenType::IDENTIFIER) {
          return Advance();
        }
        break;
    }

    throw Error(message);
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
                    TokenType::FLOAT , TokenType::DOUBLE , TokenType::TRUE , TokenType::FALSE ,
                    TokenType::STRINGL , TokenType::CHARL } , advance);
  }

  bool Parser::MatchTypes(bool advance) {
    return Match({ TokenType::I8 , TokenType::I16 , TokenType::I32 , TokenType::I64 , 
                TokenType::U8 , TokenType::U16 , TokenType::U32 , TokenType::U64 , 
                TokenType::F32 , TokenType::F64 , TokenType::STRING , TokenType::CHAR } , advance);
  }
      
  bool Parser::DeclaredType(const Token& type) const {
    for (int32_t i = current_scope; i >= 0; i--) {
      for (const auto& t : declarations[current_scope].types) {
        if (t.value == type.value) {
          return true;
        }
      }
    }

    return false;
  }

  bool Parser::DeclaredVar(const Token& var) const {
    for (int32_t i = current_scope; i >= 0; i--) {
      for (const auto& v : declarations[i].vars) {
        if (v.value == var.value) {
          return true;
        }
      }
    }

    return false;
  }

  bool Parser::DeclaredFunc(const Token& func) const {
    for (int32_t i = current_scope; i > 0; i--) {
      for (const auto& f : declarations[i].funcs) {
        if (f.value == func.value) {
          return true;
        }
      }
    }

    return false;
  }

  std::string Parser::GenerateClosureName() {
    return fmtstr("<anon_{}>", closure_counter++);
  }

  void Parser::Sync() {
    printerr(ErrorType::PARSER , fmtstr("Syncing parser, discarding {} = {}" , TokenTypeStrings[Peek().type] , Peek().value));

    while (!AtEnd()) {
      /// open brace is peek not previoud because we can parse a new block
      if (Peek().type == TokenType::OPEN_BRACE || Peek().type == TokenType::END) {
        printerr(ErrorType::PARSER , "Syncing parser, found open brace");
        return;
      }

      /// here we abandon the lost statement and try to parse a new one
      if (Previous().type == TokenType::CLOSE_BRACE ||
          Previous().type == TokenType::SEMICOLON) {
        printerr(ErrorType::PARSER , "Syncing parser, found close brace or semicolon");
        return;
      }

      switch (Peek().type) {
        default:
          Advance();
      }
    }
  }
  
  Stmt* Parser::RecoverFrom(TokenType type , std::vector<Stmt*>& stmts) {
    switch (type) {
      case TokenType::CLOSE_BRACE:
        return new BlockStmt(stmts);
      case TokenType::SEMICOLON:
      case TokenType::OPEN_BRACE: {
        return ParseBlock();
      } break;
      case TokenType::END:
        throw TokenType::END;
      default:
        throw Error(fmtstr("Unexpected token found {} = {}. Failed to recover" , TokenTypeStrings[Peek().type] , Peek().value));
    } 
  }
  
  ParserError Parser::Error(const std::string& message) const {
    SourceLocation loc = Peek().loc; 
    loc.index = current;
    return ParserError(loc , message);
  }

} // namespace ylang
