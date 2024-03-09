#include "ylc/sym_extractor.hpp"

#include "errors.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {

  void SymbolExtractor::Visit(LiteralExpr &expr) {
    if (need_identifier) {
      if (expr.value.type != TokenType::STRINGL && expr.value.type != TokenType::IDENTIFIER) {
        throw InternalError("Unable to write string literal or identifier to id stack");
      }
      identifier_stack.push(expr.value.value);
      return;
    }

    if (expr.value.type == TokenType::STRINGL) {
      std::string sym_name = fmtstr("@strl({})" , expr.value.value); 
      DataSymbol& sym = table.DefineString(sym_name , expr.value.value);
      value_stack.push(Value::CreateValue(sym.address));
      return;
    }

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

    if (var_decl) {
      return;
    }

    for (auto &arg : expr.args) {
      arg->Accept(*this);
    }
  }
  
  void SymbolExtractor::Visit(GroupingExpr &expr) { 
    expr.expr->Accept(*this); 
  }
  
  void SymbolExtractor::Visit(VarExpr &expr) {
    if (current_func.has_value()) {
      FunctionSymbol* func = table.RetrieveFunction(current_func.value());
      if (func != nullptr) {
        for (const auto& p : func->parameters) {
          if (p.name == expr.name.value) {
            return;
          }
        }
      } else {
        throw InternalError(fmtstr("Function '{}' not found in symbol table" , current_func.value()));
      }
    }

    DataSymbol* sym = table.RetrieveVariable(expr.name.value);
    if (sym != nullptr) {
      if (sym->type == Value::Type::ARRAY || sym->type == Value::Type::STRING) {
        value_stack.push(Value::CreateValue(sym->address));
      } else {
        if (sym->values.size() > 1) {
          throw InternalError(fmtstr("Variable '{}' has more than one value" , expr.name.value));
        }

        value_stack.push(sym->values[0]);
      }
      return;
    }

    FunctionSymbol* func = table.RetrieveFunction(expr.name.value);
    if (func != nullptr) {
      Value v = Value::CreateNilValue(func->return_type);
      value_stack.push(v);
      return;
    }

    return;
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

    var_decl = true;

    std::string val = "";
    std::vector<Value> values;
    if (stmt.initializer != nullptr) {
      if (stmt.type.type == TokenType::STRING) {
        need_identifier = true;
      }

      stmt.initializer->Accept(*this);

      if (need_identifier) {
        val = identifier_stack.top();
        identifier_stack.pop();
      } else {
        if (value_stack.empty() && !struct_decl) {
          throw InternalError(fmtstr("Empty value stack for variable '{}'" , sym_name));
          return;
        }

        if (struct_decl) {
          while (!value_stack.empty()) {
            values.push_back(value_stack.top());
            value_stack.pop();
          }
        } else {
          values.push_back(value_stack.top());
          value_stack.pop();
        }
      }
    } else {
      if (stmt.type.type == TokenType::STRING) {
        throw InternalError("String variable must be initialized");
      }

      if (need_identifier) {
        DataSymbol& sym = table.DefineString(sym_name , "");
        sym.type = Value::Type::STRING;
        need_identifier = false;
        return;
      }
    }

    var_decl = false;

    if (need_identifier) {
      DataSymbol& sym = table.DefineString(sym_name , val);
      sym.type = Value::Type::STRING;
      need_identifier = false;
      return;
    }

    if (struct_decl) {
      if (identifier_stack.empty()) {
        throw InternalError("Empty identifier stack for struct field");
      }

      if (values.empty()) {
        values.push_back(Value::CreateNilValue(Value::GetType(stmt.type)));
      }

      table.AddField(identifier_stack.top(), sym_name , values);
      return;
    }

    DataSymbol& sym = table.DefineVariable(sym_name , values);
    sym.type = Value::GetType(stmt.type);
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

    if (struct_decl) {
      table.AddField(identifier_stack.top(), sym_name , values);
      return;
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

    if (!value_stack.empty()) {
      value_stack.pop();
    }
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

      if (!value_stack.empty()) {
        value_stack.pop();
      }

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
  
    current_func = stmt.name.value;

    scope_name_stack.push(stmt.name.value);
    stmt.body->Accept(*this);
    scope_name_stack.pop();

    current_func = std::nullopt;
  }
  
  void SymbolExtractor::Visit(StructStmt &stmt) {
    Symbol &sym = table.DefineStruct(stmt.name.value);
    sym.type = SymbolType::STRUCT;

    struct_decl = true;
    identifier_stack.push(stmt.name.value);

    std::vector<Value> values;
    for (auto &field : stmt.fields) {
      field->Accept(*this);
      
      if (value_stack.empty()) {
        continue;
      }
    }

    struct_decl = false;
    identifier_stack.pop();
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
      return SymbolTable(fmtstr("<{} table corrupted>" , table.AsmName()));
    }
  
    table.Validate();
    return table;
  }
      
} // namespace ylang
