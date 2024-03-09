#include "ylc/sym_extractor.hpp"

#include "errors.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {

  void SymbolExtractor::Visit(LiteralExpr &expr) {
    value_stack.push(Value::CreateValue(expr.value));
  }
  
  void SymbolExtractor::Visit(UnaryExpr &expr) {
    expr.right->Accept(*this);
  }
  
  void SymbolExtractor::Visit(BinaryExpr &expr) {
    expr.left->Accept(*this);
    expr.right->Accept(*this);
  } 
  
  void SymbolExtractor::Visit(CallExpr &expr) {
    expr.callee->Accept(*this);  

    for (auto &arg : expr.args) {
      arg->Accept(*this);
    }
  }
  
  void SymbolExtractor::Visit(GroupingExpr &expr) { 
    expr.expr->Accept(*this); 
  }
  
  void SymbolExtractor::Visit(VarExpr &expr) {
  }
  
  void SymbolExtractor::Visit(AssignExpr &expr) {
  }
  
  void SymbolExtractor::Visit(ArrayExpr &expr) {
    for (auto &elem : expr.elements) {
      elem->Accept(*this);

      Value value = value_stack.top();
      value_stack.pop();

      value_stack.push(value);
    }
  }
  
  void SymbolExtractor::Visit(ArrayAccessExpr &expr) {}
  
  void SymbolExtractor::Visit(ObjAccessExpr &expr) {}
  
  void SymbolExtractor::Visit(ExprStmt &stmt) {
    stmt.expr->Accept(*this);
  }
  
  void SymbolExtractor::Visit(VarDeclStmt &stmt) {
    std::string sym_name = BuildScopeName(stmt.name.value); 
    std::vector<Value> values;
    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);

      while (!value_stack.empty()) {
        values.push_back(value_stack.top());
        value_stack.pop();
      }
    }

    DataSymbol& sym = table.DefineVariable(sym_name , values);
    sym.type = Value::GetType(stmt.type);
    sym.size = sym.values.size();
  }
  
  void SymbolExtractor::Visit(ArrayDeclStmt &stmt) {
    std::string sym_name = BuildScopeName(stmt.name.value);
    std::vector<Value> values;
    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);

      while (!value_stack.empty()) {
        values.push_back(value_stack.top());
        value_stack.pop();
      }
    }

    DataSymbol& sym = table.DefineVariable(sym_name , values);
    sym.type = Value::GetType(stmt.type);
    sym.size = sym.values.size();
  }
  
  void SymbolExtractor::Visit(BlockStmt &stmt) {
    for (auto &stmt : stmt.statements) {
      stmt->Accept(*this);
    }
  }
  
  void SymbolExtractor::Visit(PrintStmt &stmt) {
    stmt.expr->Accept(*this);
  }
  
  void SymbolExtractor::Visit(IfStmt &stmt) {
    stmt.condition->Accept(*this);
    stmt.then_branch->Accept(*this);
    if (stmt.else_branch != nullptr) {
      stmt.else_branch->Accept(*this);
    }
  }
  
  void SymbolExtractor::Visit(WhileStmt &stmt) {
    stmt.condition->Accept(*this);
    stmt.body->Accept(*this);
  }
  
  void SymbolExtractor::Visit(ReturnStmt &stmt) {
    if (stmt.stmt != nullptr) {
      stmt.stmt->Accept(*this);
    }
  }
  
  void SymbolExtractor::Visit(FunctionStmt &stmt) {
    std::string sym_name = BuildScopeName(stmt.name.value);
    FunctionSymbol &sym = table.DefineFunction(sym_name);
    sym.return_type = Value::GetType(stmt.type);

    for (auto &param : stmt.params) {
      Parameter p = {
        .type = Value::GetType(param) ,
        .name = param.value ,
        .size = Value::GetTypeSize(p.type) ,
        .default_values = {} 
      };
      sym.parameters.push_back(p); 
    }
  
    scope_name_stack.push(stmt.name.value);
    stmt.body->Accept(*this);
    scope_name_stack.pop();
  }
  
  void SymbolExtractor::Visit(StructStmt &stmt) {
    Symbol &sym = table.DefineStruct(stmt.name.value);
    printfmt(" -- Defining struct : {}", stmt.name.value);
    sym.type = SymbolType::STRUCT;
  
    for (auto &field : stmt.fields) {
      field->Accept(*this);
  
      std::string id = identifier_stack.top();
      identifier_stack.pop();
  
      printfmt(" -- Adding field : {}", id);
    }
  }

  std::string SymbolExtractor::BuildScopeName(const std::string& name) {
    std::string sym_name = name;
    if (!scope_name_stack.empty()) {
      std::stack<std::string> temp_stack;

      while (!scope_name_stack.empty()) {
        std::string scope_name = scope_name_stack.top();
        scope_name_stack.pop();
        temp_stack.push(scope_name);

        sym_name = scope_name + "::" + sym_name;
      }

      while (!temp_stack.empty()) {
        scope_name_stack.push(temp_stack.top());
        temp_stack.pop();
      }
    }

    return sym_name;
  }
  
  SymbolTable TableBuilder::Build() {
    SymbolExtractor extractor(table);
    try {
      stmts.root->Accept(extractor);
    } catch (const std::exception &e) {
      printerr(ErrorType::COMPILER , fmtstr("Error extracting symbols : {}", e.what()));
      return SymbolTable("<corrupted>");
    }
  
    return table;
  }
      
} // namespace ylang
