#include "parser/resolver.hpp"

#include "defines.hpp"
#include "errors.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {

  ///// DeclarationResolver /////
  
  void DeclarationResolver::Resolve() {
    Resolve(stmts.root);
  }

  void DeclarationResolver::Visit(LiteralExpr& expr) {
    if (expr.value.type == TokenType::STRINGL) {
      type_stack.push(Value::Type::STRING);
      return;
    }

    Value::Type type = Value::GetType(expr.value);
    type_stack.push(type);
  }
  
  void DeclarationResolver::Visit(UnaryExpr& expr) {
    Resolve(expr.right);
  }

  void DeclarationResolver::Visit(BinaryExpr& expr) {
    Resolve(expr.left);
    Resolve(expr.right);
  }

  void DeclarationResolver::Visit(CallExpr& expr) {
    need_identifier = true;
    Resolve(expr.callee);
    need_identifier = false;

    if (token_stack.empty()) {
      throw Error(Token() , "Call Expr has no callee" , ErrorType::INTERNAL);
    }

    Token name = token_stack.top();

    Value::Type callee_type = GetDeclaredType(name);
    type_stack.push(callee_type);

    for (auto& arg : expr.args) {
      Resolve(arg);

      if (type_stack.empty()) {
        throw Error(Token() , "Type stack is empty in Call Expr" , ErrorType::INTERNAL);
      }

      Value::Type arg_type = type_stack.top();
      type_stack.pop();

      if (!Value::TypesCompatible(callee_type , arg_type)) {
        throw Error(name , fmtstr("Function '{}' called with argument of type '{}' but expected '{}'" , 
                                  name.value , kValueStrings[arg_type] , kValueStrings[callee_type]));
      }
    }
  }

  void DeclarationResolver::Visit(GroupingExpr& expr) {
    Resolve(expr.expr);
  }

  void DeclarationResolver::Visit(VarExpr& expr) {
    ResolveLocal(expr , expr.name);
    
    if (need_identifier) {
      token_stack.push(expr.name);
    }

    if (IsFunction(expr.name.value)) {
      SetType(expr.name , Value::Type::CALLABLE);
      type_stack.push(Value::Type::ADDRESS);
      return;
    }

    Value::Type type = GetDeclaredType(expr.name);
    type_stack.push(type);
  }

  void DeclarationResolver::Visit(AssignExpr& stmt) {
    Value::Type decl_type = GetDeclaredType(stmt.name);

    Resolve(stmt.value);
    ResolveLocal(stmt , stmt.name);
    
    if (type_stack.empty()) {
      throw Error(Token() , "Type stack is empty in Assignment" , ErrorType::INTERNAL);
    }

    bool is_call = stmt.value->GetType() == CALL_EXPR;
    Token func_name;
    if (is_call) {
      func_name = token_stack.top();
      token_stack.pop();
    }

    Value::Type value_type = is_call ? 
      GetFunctionType(func_name.value) : type_stack.top();
    
    if (is_call) {
      type_stack.pop();
    }

    if (!Value::TypesCompatible(decl_type , value_type)) {
      throw Error(stmt.name , fmtstr("Variable '{}' declared as '{}' but assigned as '{}'" , 
                                      stmt.name.value , kValueStrings[decl_type] , kValueStrings[value_type]));
    }
  }

  void DeclarationResolver::Visit(ArrayExpr& stmt) {
    for (auto& expr : stmt.elements) {
      Resolve(expr);
    }
  }

  void DeclarationResolver::Visit(ArrayAccessExpr& stmt) {
    Resolve(stmt.array);
    Resolve(stmt.index);
  }

  void DeclarationResolver::Visit(ObjAccessExpr& stmt) {
    Resolve(stmt.obj);
    Resolve(stmt.index);
  }
  
  void DeclarationResolver::Visit(ExprStmt& stmt) {
    Resolve(stmt.expr);
  }
  
  void DeclarationResolver::Visit(VarDeclStmt& stmt) {
    Declare(stmt.name , stmt.type);

    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);

      if (type_stack.empty()) {
        throw Error(Token() , "Token stack is empty in Variable Declaration" , ErrorType::INTERNAL);
      }

      Token func_name;
      if (stmt.initializer->GetType() == CALL_EXPR) {
        func_name = token_stack.top();
        token_stack.pop();
      }

      Value::Type declared_type = stmt.initializer->GetType() == CALL_EXPR ? 
          GetFunctionType(func_name.value) : 
          GetDeclaredType(stmt.name);

      Value::Type initializer_type_enum = type_stack.top();
      type_stack.pop();

      stmt.type.type = Value::GetTokenType(declared_type);

      if (declared_type == Value::Type::VOID) {
        SetType(stmt.name , initializer_type_enum);
        stmt.type = Token(stmt.type.loc , Value::GetTokenType(initializer_type_enum) , stmt.type.value);
      } else if (!Value::TypesCompatible(declared_type , initializer_type_enum)){
        throw Error(stmt.name , fmtstr("Variable '{}' declared as '{}' but initialized as '{}'" , 
                                        stmt.name.value , kValueStrings[declared_type] , kValueStrings[initializer_type_enum]));
      }
    }

    Define(stmt.name , stmt.type);
  }

  void DeclarationResolver::Visit(ArrayDeclStmt& stmt) {
    Declare(stmt.name , stmt.type);

    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);
    }

    Define(stmt.name , stmt.type);
  }

  void DeclarationResolver::Visit(BlockStmt& stmt) {
    if (stmt.statements.empty()) {
      return;
    }

    BeginScope();
    Resolve(stmt.statements); 
    EndScope();

    if (stmt.statements.empty() || stmt.statements.back()->GetType() != NodeType::RETURN_STMT) {
      type_stack.push(Value::Type::VOID);
    }
  }
  
  void DeclarationResolver::Visit(PrintStmt& stmt) {
    if (stmt.expr != nullptr) {
      Resolve(stmt.expr);
    }
  }

  void DeclarationResolver::Visit(IfStmt& stmt) {
    Resolve(stmt.condition);
    Resolve(stmt.then_branch);
    if (stmt.else_branch != nullptr) {
      Resolve(stmt.else_branch);
    }
  }

  void DeclarationResolver::Visit(WhileStmt& stmt) {
    Resolve(stmt.condition);
    Resolve(stmt.body);
  }

  void DeclarationResolver::Visit(ReturnStmt& stmt) {
    if (stmt.stmt != nullptr) {
      Resolve(stmt.stmt);
      if (stmt.stmt->GetType() == NodeType::FUNCTION_STMT) {
        type_stack.pop();
        type_stack.push(Value::Type::CALLABLE);
      } 
    } else {
      while (!type_stack.empty()) {
        type_stack.pop();
      }
      type_stack.push(Value::Type::VOID);
    }
  }

  void DeclarationResolver::Visit(FunctionStmt& stmt) {
    Declare(stmt.name , stmt.type);
    Define(stmt.name , stmt.type);
    DeclareFunction(stmt.name.value , Value::GetType(stmt.type));
    DefineFunction(stmt.name.value , Value::GetType(stmt.type));

    ResolveFunction(stmt);

    Value::Type return_type = type_stack.top();
    type_stack.pop();

    if (GetDeclaredType(stmt.name) == Value::Type::VOID) {
      SetType(stmt.name , return_type);
      stmt.type = Token(stmt.type.loc , Value::GetTokenType(return_type) , stmt.type.value);
    }
  }

  void DeclarationResolver::Visit(StructStmt& stmt) {
    Declare(stmt.name , stmt.name);
    Define(stmt.name , stmt.name);
  }
      
  DeclarationResolver::Variable DeclarationResolver::CreateVar(const Token& name , const Token& type) {
    Variable var;
    var.name = name.value;
    var.type = Value::GetType(type);
    return var;
  }
      
  Value::Type DeclarationResolver::GetDeclaredType(const Token& name) {
    uint64_t name_hash = FNV(name.value);

    for (int i = current_scope; i >= 0; i--) {
      for (auto& [key , var] : scopes[i]) {
        if (key == name_hash) {
          if (var.type == Value::Type::CALLABLE) {
            return GetFunctionType(name.value);
          }
          return var.type;
        }
      }
    }

    throw Error(name , fmtstr("Variable '{}' not declared in this scope" , name.value));
  }

  void DeclarationResolver::SetType(const Token& name , Value::Type type) {
    uint64_t name_hash = FNV(name.value);

    for (int i = current_scope; i >= 0; i--) {
      for (auto& [key , var] : scopes[i]) {
        if (key == name_hash) {
          var.type = type;
          return;
        }
      }
    }

    throw Error(name , fmtstr("Variable '{}' not declared in this scope" , name.value));
  }

  void DeclarationResolver::BeginScope() {
    current_scope++;
    if (current_scope >= kMaxScopeDepth) {
      throw Error(Token() , "Max scope depth reached");
    }
  }

  void DeclarationResolver::EndScope() {
    scopes[current_scope].declared_funcs.clear();
    scopes[current_scope].clear();
    current_scope--;
  }

  void DeclarationResolver::Resolve(std::vector<Stmt*>& stmts) {
    for (auto& stmt : stmts) {
      Resolve(stmt);
    }
  }

  void DeclarationResolver::Resolve(Stmt* stmt) {
    stmt->Accept(*this);
  }

  void DeclarationResolver::Resolve(Expr* expr) {
    expr->Accept(*this);
  }

  void DeclarationResolver::ResolveLocal(Expr& expr , const Token& name) {
    uint64_t name_hash = FNV(name.value);

    for (int i = current_scope; i >= 0; i--) {
      for (auto& [key , var] : scopes[i]) {
        if (key == name_hash && !var.defined) {
          throw Error(name , fmtstr("Cannot read local variable in its own initializer"));
        } else if (key == name_hash) {
          return;
        }
      }
    }

    throw Error(name , fmtstr("Variable {} not declared in this scope" , name.value));
  }

  void DeclarationResolver::ResolveFunction(FunctionStmt& name) {
    BeginScope();
    for (auto& param : name.params) {
      Declare(param , param);
      Define(param , param);
    }

    Resolve(name.body);
    EndScope();
  }

  void DeclarationResolver::ResolveType(const Token& name , const Token& type) {
    uint64_t name_hash = FNV(name.value);
    Value::Type t = Value::GetType(type);

    for (int i = current_scope; i >= 0; i--) {
      for (auto& [key , var] : scopes[i]) {
        if (key == name_hash && var.type == t) {
          return;
        } else if (key == name_hash) {
          throw Error(name , fmtstr("Type {} does not match declared type {} of '{}'" , 
                                      kValueStrings[t] , kValueStrings[var.type] , name.value));
        }
      }
    }

    throw Error(name , fmtstr("Type {} not declared in this scope" , name.value));
  }
      
  void DeclarationResolver::Declare(const Token& name , const Token& type) {
    Variable var = CreateVar(name , type);
    uint64_t name_hash = FNV(name.value);

    if (scopes[current_scope].find(name_hash) != scopes[current_scope].end()) {
      throw Error(name , fmtstr("Variable {} already declared in this scope" , name.value));
    }
    scopes[current_scope][name_hash] = var;
  }

  void DeclarationResolver::Define(const Token& name , const Token& type) {
    uint64_t name_hash = FNV(name.value);

    if (scopes[current_scope].find(name_hash) == scopes[current_scope].end()) {
      throw Error(name , fmtstr("Defining undeclared variable {} in this scope" , name.value));
    }
    scopes[current_scope][name_hash].defined = true;
  }

  void DeclarationResolver::DeclareFunction(const std::string& name , Value::Type type) {
    uint64_t hash = FNV(name);
    scopes[current_scope].declared_funcs[hash] = Function{ 
      .name = name ,
      .type = type ,
      .defined = false
    };
  }

  void DeclarationResolver::DefineFunction(const std::string& name , Value::Type type) {
    uint64_t addr = FNV(name);
    if (scopes[current_scope].declared_funcs.find(addr) == scopes[current_scope].declared_funcs.end()) {
      throw Error(Token() , fmtstr("Defining undeclared function {} in this scope" , name));
    }
    scopes[current_scope].declared_funcs[addr].defined = true;
  }

  bool DeclarationResolver::IsFunction(const std::string& name) {
    for (int i = current_scope; i >= 0; i--) {
      for (auto& [key , func] : scopes[i].declared_funcs) {
        if (func.name == name && func.defined) {
          return true;
        }
      }
    }
    return false;
  }

  Value::Type DeclarationResolver::GetFunctionType(const std::string& name) {
    for (int i = current_scope; i >= 0; i--) {
      for (auto& [key , func] : scopes[i].declared_funcs) {
        if (func.name == name && func.defined) {
          return func.type;
        }
      }
    }
    throw Error(Token() , fmtstr("Function {} not declared in this scope" , name));
  }

  void DeclarationResolver::DumpScopes() {
    for (uint32_t i = 0; i <= current_scope; i++) {
      printfmt("Scope {}" , i);
      for (auto& [name , var] : scopes[i]) {
        printfmt("Name : {} ({}) , Type : {} , Defined : {}", var.name , name , kValueStrings[var.type] , var.defined);
      }
    }
  }

  StaticAnalysisError DeclarationResolver::Error(const Token& token, const std::string& message, ErrorType type) {
    return StaticAnalysisError(token.loc, message , type);
  }

  ///// Resolver /////
  
  bool Resolver::Resolve() {
    try {
      DeclarationResolver decl_resolver(statements);
      decl_resolver.Resolve();
    } catch (const StaticAnalysisError& e) {
      printerr(e.type , e.what());
      return false;
    }

    return true;
  }

} // namespace ylang
