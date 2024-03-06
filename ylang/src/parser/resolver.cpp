#include "parser/resolver.hpp"

#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"
#include "errors.hpp"

namespace ylang {

  ///// DeclarationResolver /////
  
  void DeclarationResolver::Resolve() {
    for (auto& stmt : stmts) {
      stmt->Accept(*this);
    }
  }

  void DeclarationResolver::Visit(LiteralExpr& expr) {
    // do nothing
  }
  
  void DeclarationResolver::Visit(UnaryExpr& expr) {
    Resolve(expr.right);
  }

  void DeclarationResolver::Visit(BinaryExpr& expr) {
    Resolve(expr.left);
    Resolve(expr.right);
  }

  void DeclarationResolver::Visit(CallExpr& expr) {
    Resolve(expr.callee);

    for (auto& arg : expr.args) {
      Resolve(arg);
    }
  }

  void DeclarationResolver::Visit(GroupingExpr& expr) {
    Resolve(expr.expr);
  }

  void DeclarationResolver::Visit(VarExpr& expr) {
    ResolveLocal(expr , expr.name);
  }

  void DeclarationResolver::Visit(AssignExpr& stmt) {
    Resolve(stmt.value);
    ResolveLocal(stmt , stmt.name);
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
    print("Visiting var decl stmt");
    Declare(stmt.name);

    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);
    }

    Define(stmt.name);
  }

  void DeclarationResolver::Visit(ArrayDeclStmt& stmt) {
    Declare(stmt.name);

    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);
    }

    Define(stmt.name);
  }

  void DeclarationResolver::Visit(BlockStmt& stmt) {
    if (stmt.statements.empty()) {
      return;
    }

    BeginScope();
    Resolve(stmt.statements); 
    EndScope();
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
    if (stmt.expr!= nullptr) {
      Resolve(stmt.expr);
    }
  }

  void DeclarationResolver::Visit(FunctionStmt& stmt) {
    Declare(stmt.name);
    Define(stmt.name);

    Resolve(stmt.body);
  }

  void DeclarationResolver::Visit(StructStmt& stmt) {
    Declare(stmt.name);
    Define(stmt.name);
  }

  void DeclarationResolver::BeginScope() {
    scopes[current_scope] = std::unordered_map<std::string , bool>();
    current_scope++;
  }

  void DeclarationResolver::EndScope() {
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
    for (int i = current_scope - 1; i >= 0; i--) {
      if (scopes[i].find(name.value) != scopes[i].end()) {
        return;
      }
    }
  }

  void DeclarationResolver::ResolveFunction(FunctionStmt& name) {
    BeginScope();
    for (auto& param : name.params) {
      Declare(param);
      Define(param);
    }
    Resolve(name.body);
    EndScope();
  }
      
  void DeclarationResolver::Declare(const Token& name) {
    if (scopes.empty()) {
      return;
    }

    if (scopes[current_scope].find(name.value) != scopes[current_scope].end()) {
      throw Error(name , fmtstr("Variable {} already declared in this scope" , name.value));
    }
    scopes[current_scope][name.value] = false;
  }

  void DeclarationResolver::Define(const Token& name) {
    print("Defining variable");
    if (scopes.empty()) {
      return;
    }

    if (scopes[current_scope].find(name.value) == scopes[current_scope].end()) {
      throw Error(name , fmtstr("Defining undeclared variable {} in this scope" , name.value));
    }
    scopes[current_scope][name.value] = true;
  }

  StaticAnalysisError DeclarationResolver::Error(const Token& token, const std::string& message, ErrorType type) {
    return StaticAnalysisError(token.loc, message , type);
  }

  ///// Resolver /////
  
  bool Resolver::Resolve() {
    try {
      DeclarationResolver decl_resolver(statements.Nodes());
      decl_resolver.Resolve();
    } catch (const StaticAnalysisError& e) {
      printerr(e.type , e.what());
      return false;
    }

    return true;
  }

} // namespace ylang