#include "interpreter/interpreter.hpp"

#include "errors.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {
  
  ReturnValue Function::Call(Interpreter* interpreter , const std::vector<Value>& args) {
    if (args.size() != arity) {
      throw InterpreterError(fmtstr("Expected {} arguments but got {}", arity , args.size()));
    }

    try {
      env->Define(this);

      env->PushScope(name);
      for (uint32_t i = 0; i < arity; i++) {
        env->Define(params[i].value , args[i]);
      }

      interpreter->Execute(stmt.body , env);
    } catch (Return& ret) {
      env->Undefine(name);

      if (ret.IsCallable()) {
        Callable* ret_callable = ret.GetCallable()->Clone();
        env->PopScope();
        return ret_callable;
      }

      env->PopScope();
      return ret.GetReturn();
    } catch (InterpreterError& e) {
      env->Undefine(name);
      env->PopScope();
      throw e;
    } catch (InternalError& e) {
      env->Undefine(name);
      env->PopScope();
      throw e;
    }

    throw InternalError(fmtstr("Function {} call failed for unknown reasons", name));
  }

  void Executor::Visit(LiteralExpr& expr) {
    if (expr.value.type == TokenType::STRINGL) {
      address_t addr = env->Alloc(expr.value.value); 
      if (addr == 0) {
        throw InternalError("Failed to allocate memory for string literal");
      }

      Value value = Value::CreateValue(addr);
      value.type = Value::Type::STRING;
      value_stack.push(value);
      return;
    }  


    Value value = Value::CreateValue(expr.value); 
    value_stack.push(value);
  }

  void Executor::Visit(UnaryExpr& expr) {}

  void Executor::Visit(BinaryExpr& expr) {
    expr.left->Accept(*this);
    expr.right->Accept(*this);

    if (value_stack.empty()) {
      throw InterpreterError("Invalid right operand for binary expression");
    }

    Value right = value_stack.top();
    value_stack.pop();

    if (value_stack.empty()) {
      throw InterpreterError("Invalid left operand for binary expression");
    }

    Value left = value_stack.top();
    value_stack.pop();

    switch (expr.op.type) {
      case TokenType::PLUS:
        if (left.type == Value::Type::STRING || right.type == Value::Type::STRING) {
          ConcatStrings(left , right);
          return;
        }
        value_stack.push(left + right);
        break;
      case TokenType::MINUS:
        value_stack.push(left - right);
        break;
      case TokenType::STAR:
        value_stack.push(left * right);
        break;
      case TokenType::F_SLASH:
        value_stack.push(left / right);
        break;
      case TokenType::EQUAL_EQUAL: {
        bool res;
        if (left.type == Value::Type::STRING && right.type == Value::Type::STRING) {
          res = CompareStrings(left , right);
        } else if ((left.type == Value::Type::ARRAY || left.type == Value::Type::STRING) || 
                   (right.type == Value::Type::ARRAY || right.type == Value::Type::STRING)) {
          res = CompareArrays(left , right);
        } else {
          res = left == right;
        }
        value_stack.push(Value::CreateValue(res));
      } break;
      case TokenType::BANG_EQUAL: {
        bool res = left != right;
        value_stack.push(Value::CreateValue(res));
      } break;
      case TokenType::GREATER: {
        bool res = left > right;
        value_stack.push(Value::CreateValue(res));
      } break;
      case TokenType::GREATER_EQUAL: {
        bool res = left >= right;
        value_stack.push(Value::CreateValue(res));
      } break;
      case TokenType::LESS: {
        bool res = left < right;
        value_stack.push(Value::CreateValue(res));
      } break;
      case TokenType::LESS_EQUAL: {
        bool res = left <= right;
        value_stack.push(Value::CreateValue(res));
      } break;
      case TokenType::LOGICAL_AND: {
        bool res = left && right;
        value_stack.push(Value::CreateValue(res));
      } break;
      case TokenType::LOGICAL_OR: {
        bool res = left || right;
        value_stack.push(Value::CreateValue(res));
      } break;
      default:
        throw InterpreterError(fmtstr("Invalid binary operator ({})" , expr.op.value));
    }
  }

  void Executor::Visit(CallExpr& expr) {
    need_identifier = true;
    expr.callee->Accept(*this);
    need_identifier = false;

    if (identifier_stack.empty()) {
      throw InternalError("No identifier on stack for call expression");
    }

    std::string callee = identifier_stack.top();
    identifier_stack.pop();

    std::vector<Value> args;
    for (auto& arg : expr.args) {
      arg->Accept(*this);
      args.push_back(value_stack.top());
      value_stack.pop();
    }

    Callable* callable = env->GetCallable(callee);

    interpreter->PushCall(callee);
    call_stack.push(callable);
    ReturnValue ret = callable->Call(interpreter , args);

    switch (ret.index()) {
      case kValueIndex: {
        Value value = std::get<Value>(ret);
        value_stack.push(value); 
      } break;
      case kCallableIndex: {
        Callable* ret_callable = std::get<Callable*>(ret)->Clone();
        callable_stack.push(ret_callable);
      } break;
      case kVoidIndex: break;
      default:
        throw InternalError("UNREACHABLE : Invalid ReturnValue index");
    }
  }

  void Executor::Visit(GroupingExpr& expr) {
    expr.expr->Accept(*this);
  }

  void Executor::Visit(VarExpr& expr) {
    if (need_identifier) {
      identifier_stack.push(expr.name.value);
      return;
    } 

    Variable var = env->Get(expr.name.value);
    if (var.val.type == Value::Type::NIL) {
      Callable* callable = env->GetCallable(expr.name.value);
      call_stack.push(callable);
      return;
    }

    value_stack.push(var.val);
  }

  void Executor::Visit(AssignExpr& expr) {
    if (!env->Defined(expr.name.value)) {
      throw InterpreterError(fmtstr("Assigning to undefined variable {}" , expr.name.value));
    }

    expr.value->Accept(*this);
    env->Assign(expr.name.value , value_stack.top());
    value_stack.pop();
  }

  void Executor::Visit(ArrayExpr& expr) {
    if (identifier_stack.empty()) {
      throw InternalError("No identifier on stack for array expression");
    }

    std::string name = identifier_stack.top();
    identifier_stack.pop();

    Value addr_val = Value::CreateValue((address_t)0);
    addr_val.type = Value::Type::ARRAY;
    if (expr.elements.empty()) {
      value_stack.push(addr_val);
      return;
    }

    std::vector<Value> array(expr.elements.size());
    for (uint32_t i = 0; i < expr.elements.size(); i++) {
      expr.elements[i]->Accept(*this);
      if (!need_identifier) {
        array[i] = value_stack.top();
        value_stack.pop();
      }
    }


    if (need_identifier) {
      value_stack.push(addr_val);
      return;
    }

    address_t addr = env->Alloc(array.data() , expr.elements.size() , name);
    if (addr == 0) {
      throw InternalError("Failed to allocate memory for array");
    }

    addr_val.value = addr;
    value_stack.push(addr_val);
  }

  void Executor::Visit(ArrayAccessExpr& expr) {
    if (expr.array == nullptr) {
      throw InternalError("Array access expression has no array");
    }
    if (expr.index == nullptr) {
      throw InternalError("Array access expression has no index");
    }

    need_identifier = true;
    expr.array->Accept(*this);
    need_identifier = false;

    if (identifier_stack.empty()) {
      throw InternalError("No identifier on stack for array access expression");
    }

    std::string array = identifier_stack.top();
    identifier_stack.pop();

    Value addr = env->Get(array).val;
    printfmt("Array access : {}" , addr);
    if (addr.type != Value::Type::ARRAY && addr.type != Value::Type::STRING) {
      throw InterpreterError(fmtstr("Invalid array access on non-array variable {}" , array));
    }

    Variable* var = env->Ref(addr.AsAddress());
    if (var == nullptr) {
      throw InternalError(fmtstr("Invalid array access on non-array variable {}" , array));
    }

    expr.index->Accept(*this);
    Value index = value_stack.top();
    value_stack.pop();

    if (!index.IsInt()) {
      throw InterpreterError("Invalid array index");
    } 

    if (index.IsNegative()) {
      throw InterpreterError("Array index cannot be negative");
    }

    if (index.AsUInt() < 0 || index.AsUInt() >= var->total_size) {
      throw InterpreterError("Array index out of bounds");
    }

    Value value = env->Ref(addr.AsAddress() + index.AsUInt())->val;
    value_stack.push(value);
  }

  void Executor::Visit(ObjAccessExpr& expr) {
    if (expr.obj == nullptr) {
      throw InternalError("Object access expression has no object");
    }

    need_identifier = true;
    expr.obj->Accept(*this);
    need_identifier = false;

    if (identifier_stack.empty()) {
      throw InternalError("No identifier on stack for object access expression");
    }

    std::string object = identifier_stack.top();
    identifier_stack.pop();

    Value addr = env->Get(object).val;
    if (addr.type != Value::Type::STRUCT) {
      throw InterpreterError(fmtstr("Invalid object access on non-struct variable {}" , object));
    }
  
    Variable* var = env->Ref(addr.AsAddress());
    if (var == nullptr) {
      throw InternalError(fmtstr("Invalid object access on non-struct variable {}" , object));
    }

    size_t scope_begin = var->name.find_first_of("::");
    std::string struct_name = var->name.substr(0 , scope_begin); 

    Symbol* symbol = env->Symbols().RetrieveSymbol(struct_name);

    Field* field = nullptr;
    for (size_t i = 0; i < symbol->fields.size(); i++) {
      if (symbol->fields[i].name == expr.member.value) {
        field = &symbol->fields[i];
        break;
      }
    }

    if (field == nullptr) {
      throw InterpreterError(fmtstr("Struct {} has no field {}" , struct_name , expr.member.value));
    }

    Value index = Value();
    if (expr.index != nullptr) {
      expr.index->Accept(*this);
      index = value_stack.top();
      value_stack.pop();
    }

    if (index.type != Value::Type::NIL) {
      if (!index.IsInt()) {
        throw InterpreterError("Invalid array index");
      } 

      if (index.IsNegative()) {
        throw InterpreterError("Array index cannot be negative");
      }

      if (index.AsUInt() < 0 || index.AsUInt() >= field->size) {
        throw InterpreterError("Array index out of bounds");
      }
    }

    if (expr.assignment != nullptr) {
      expr.assignment->Accept(*this);
      Value value = value_stack.top();
      value_stack.pop();

      if (index.type != Value::Type::NIL) {
        address_t field_start = addr.AsAddress() + field->offset;
        env->Ref(field_start + index.AsUInt())->val = value;
        return;
      }

      env->Ref(addr.AsAddress() + field->offset)->val = value;
      return;
    } else {
      
      if (index.type != Value::Type::NIL) {
        address_t field_start = addr.AsAddress() + field->offset;
        Value val = env->Ref(field_start + index.AsUInt())->val;
        value_stack.push(val);
        return;
      }

      address_t field_start = addr.AsAddress() + field->offset;
      Value val = env->Ref(field_start)->val;
      value_stack.push(val);
    }

  }

  void Executor::Visit(ExprStmt& stmt) {
    stmt.expr->Accept(*this);
  }

  void Executor::Visit(VarDeclStmt& stmt) {
    if (need_identifier) {
      identifier_stack.push(stmt.name.value);
      if (stmt.initializer != nullptr) {
        stmt.initializer->Accept(*this);
      }
      return;
    }

    if (stmt.name.value == "DUMP") {
      env->Dump();
      return;
    }

    if (env->Symbols().SymbolExists(stmt.type.value)) {
      address_t addr = AllocateStruct(stmt.type.value);

      Value value = Value::CreateValue(addr);
      value.type = Value::Type::STRUCT;
      env->Define(stmt.name.value , value);

      return;
    }

    if (stmt.initializer != nullptr) {
      stmt.initializer->Accept(*this);

      if (!value_stack.empty()) {
        bool reassigining = env->Defined(stmt.name.value);

        Value value = value_stack.top();
        value_stack.pop();

        if (reassigining) {
          env->Assign(stmt.name.value , value);
        } else {
          env->Define(stmt.name.value , value);
        }
      } else if (!call_stack.empty()) {
        Callable* callable = call_stack.top();
        call_stack.pop();

        /// rename the callable to the variable being declared
        callable->name = stmt.name.value;
        env->Define(callable);
      } else {
        throw InternalError("No value on stack while evaluating variable declaration");
      }
    }
  }

  void Executor::Visit(ArrayDeclStmt& stmt) {
    identifier_stack.push(stmt.name.value);
    if (stmt.initializer != nullptr) {
      identifier_stack.push(stmt.name.value);
      stmt.initializer->Accept(*this);
    }
    
    size_t real_size = std::stoull(stmt.size.value);

    if (need_identifier) {
      Value::Type arr_type = env->Symbols().StringToType(stmt.type.value);
      type_stack.push(arr_type);
      if (value_stack.size() - 1 != real_size) {
        Value addr_val = value_stack.top();
        value_stack.pop();

        size_t nil_values = real_size - value_stack.size();
        for (size_t i = 0; i < nil_values; i++) {
          value_stack.push(Value());
        }

        value_stack.push(addr_val);
      }
      return;
    }

    if (value_stack.empty()) {
      throw InternalError("No value on stack while evaluating array declaration");
    }

    Value value = value_stack.top();
    value_stack.pop();
    env->Define(stmt.name.value , value);

    Variable* var = env->Ref(value.AsAddress());
    if (var->total_size != real_size) {
      size_t nils = real_size - var->total_size;
      for (size_t i = 0; i < nils; i++) {
        env->Alloc(Value() , stmt.name.value);
      }
    } 
  }

  void Executor::Visit(BlockStmt& stmt) {
    for (auto& s : stmt.statements) {
      s->Accept(*this);
    }
  }

  void Executor::Visit(PrintStmt& stmt) {
    stmt.expr->Accept(*this);

    if (!value_stack.empty()) {
      Value value = value_stack.top();
      value_stack.pop();

      if (value.IsNull()) {
        printfmt("nil");
      } else if (value.IsBool()) {
        bool val = value.AsBool();
        printfmt("{}", val);
      } else if (value.IsChar()) {
        char val = value.AsChar();
        printfmt("{}", val);
      } else if (value.IsUnsigned()) {
        uint64_t val = value.AsUInt();
        printfmt("{}", val);
      } else if (value.IsSigned()) {
        int64_t val = value.AsSInt();
        printfmt("{}", val);
      } else if (value.IsFloat()) {
        double val = value.AsFloat();
        printfmt("{}", val);
      } else if (value.IsDouble()) {
        double val = value.AsDouble();
        printfmt("{}", val);
      } else if (value.IsAddress()) {
        address_t val = value.AsAddress();
        printfmt("{:#08x}", val.address);
      } else if (value.IsString()) {
        std::string val = env->RefString(value.AsAddress());
        printfmt("{}", val);
      } else if (value.IsArray()) {
        address_t base_addr = value.AsAddress();

        Variable* var = env->Ref(base_addr); 
        size_t size = var->total_size;

        std::string str = fmtstr("[{}] : [", size);
        for (size_t i = 0; i < size; i++) {
          Value val = env->Ref(base_addr + i)->val;
          str += fmtstr("{} " , val.AsString());
          if (i != size - 1) {
            str += " , ";
          }
        }
        str += "]";
        printfmt("{}", str);
      } else {
        throw InterpreterError(fmtstr("Invalid value type for print statement : {}", value));
      }

    } else {
      throw InternalError("No value on stack while evaluating print statement");
    }
  }

  void Executor::Visit(IfStmt& stmt) {
    stmt.condition->Accept(*this);

    if (!value_stack.empty()) {
      Value value = value_stack.top();
      value_stack.pop();

      if (value.type != Value::Type::BOOL) {
        throw InterpreterError(fmtstr("Expected boolean condition but got {}", value));
      }

      if (value.AsBool()) {
        stmt.thenBranch->Accept(*this);
      } else if (stmt.elseBranch != nullptr) {
        stmt.elseBranch->Accept(*this);
      }
    } else {
      throw InternalError("No value on stack while evaluating if condition");
    }
  }

  void Executor::Visit(WhileStmt& stmt) {
    stmt.condition->Accept(*this);

    if (!value_stack.empty()) {
      Value value = value_stack.top();
      value_stack.pop();

      if (value.type != Value::Type::BOOL) {
        throw InterpreterError(fmtstr("Expected boolean condition but got {}", value));
      }

      while (value.AsBool()) {
        stmt.body->Accept(*this);
        stmt.condition->Accept(*this);

        if (!value_stack.empty()) {
          value = value_stack.top();
          value_stack.pop();
        } else {
          throw InternalError("No value on stack while evaluating while condition");
        }
      }
    } else {
      throw InternalError("No value on stack while evaluating while condition");
    }
  }

  void Executor::Visit(ReturnStmt& stmt) {
    if (stmt.expr != nullptr) {
      returning = true;
      stmt.expr->Accept(*this);
      returning = false;

      if (!value_stack.empty()) {
        Value value = value_stack.top();
        value_stack.pop();

        throw Return(value);
      } else if (!call_stack.empty()) {
        Callable* callable = call_stack.top();
        call_stack.pop();

        throw Return(callable);
      }

      throw InternalError("No value on stack while evaluating return statement");
    }
    throw Return(Value()); 
  }

  void Executor::Visit(FunctionStmt& stmt) {
    if (env->Defined(stmt.name.value) && env->IsVar(stmt.name.value)) {
      Variable var = env->Get(stmt.name.value);
      if (var.val.type != Value::Type::NIL) {
        throw InterpreterError(fmtstr("Identifier {} already defined as variable {}" , stmt.name.value , var.val));
      }

      env->Undefine(stmt.name.value);
    }

    auto closure = Environment::CreateClosure(env);
    Callable* callable = new Function(stmt , closure);
    env->Define(callable);
  }

  void Executor::Visit(StructStmt& stmt) {
    auto& table = env->Symbols();
    auto& symbol = table.DefineStruct(stmt.name.value);


    need_identifier = true;
    for (auto& field : stmt.fields) {
      field->Accept(*this);

      std::string name = identifier_stack.top();
      identifier_stack.pop();

      Value value = value_stack.top();
      value_stack.pop();

      std::vector<Value> initial_values;
      if (value.IsArray()) {
        Value::Type type = type_stack.top();

        // necessary because stack means values are backwards
        std::vector<Value> temp{};
        while (!value_stack.empty()) {
          value_stack.top().type = type;
          size_t size = env->Symbols().TypeSize(type);
          if (size <= 8 ) {
            value_stack.top().size = WordSize{ size };
          }

          temp.push_back(value_stack.top());
          value_stack.pop();
        }

        for (auto it = temp.rbegin(); it != temp.rend(); it++) {
          initial_values.push_back(*it);
        }
      } else {
        initial_values.push_back(value);
      }

      table.AddField(symbol , name , initial_values);
    }
    need_identifier = false;
  }
      
  void Executor::ConcatStrings(const Value& lhs , const Value& rhs) {
    std::string left = lhs.AsString();
    std::string right = rhs.AsString();
    std::string result;

    if (lhs.IsString() && rhs.IsString()) {
      left = env->RefString(lhs.AsAddress());
      right = env->RefString(rhs.AsAddress());
      result = left + right;

    } else if (lhs.IsString()) {
      left = env->RefString(lhs.AsAddress());

    } else if (rhs.IsString()) {
      std::string right = env->RefString(rhs.AsAddress());
      std::string result = lhs.AsString() + right;
    } else {
      throw InterpreterError(fmtstr("Invalid types for string concatenation : {} and {}" , lhs , rhs));
    }

    result = left + right;
    address_t res_addr = env->Alloc(result);
    Value res_val = Value::CreateValue(res_addr);
    res_val.type = Value::Type::STRING;
    value_stack.push(res_val);
  }

  bool Executor::CompareStrings(const Value& lhs , const Value& rhs) {
    std::string left = lhs.AsString();
    std::string right = rhs.AsString();

    if (lhs.IsString() && rhs.IsString()) {
      left = env->RefString(lhs.AsAddress());
      right = env->RefString(rhs.AsAddress());
    } else {
      return false;
    }

    return left == right;
  }
      
  bool Executor::CompareArrays(const Value& lhs , const Value& rhs) {
    if ((lhs.IsArray() || lhs.IsString()) && 
        (rhs.IsArray() || rhs.IsString())) {
      address_t lhs_addr = lhs.AsAddress();
      address_t rhs_addr = rhs.AsAddress();

      Variable* lhs_var = env->Ref(lhs_addr);
      Variable* rhs_var = env->Ref(rhs_addr);

      size_t check_size = lhs_var->total_size < rhs_var->total_size ? 
        lhs_var->total_size : rhs_var->total_size;

      for (size_t i = 0; i < check_size; i++) {
        Value lhs_val = env->Ref(lhs_addr + i)->val;
        Value rhs_val = env->Ref(rhs_addr + i)->val;

        if (lhs_val != rhs_val) {
          return false;
        }
      }

      return true;
    }

    return false;
  }
      
  address_t Executor::AllocateStruct(const std::string& sym_name) {
    auto& table = env->Symbols();
    if (!table.SymbolExists(sym_name)) {
      throw InterpreterError(fmtstr("Undefined struct : {}" , sym_name));
    }

    return env->Alloc(table , sym_name);
  }

  ExitCode Interpreter::Interpret() {
    global_env = std::make_unique<Environment>();
    Executor emitter(this , global_env);

    for (auto& stmt : ast.Nodes()) {
      try {
        stmt->Accept(emitter);
      } catch (const Return& e) {
        if (e.IsVoid()) {
          return ExitCode::OK;
        }

        switch (e.Index()) {
          case kValueIndex: printfmt("RET {}" , e.GetValue()); break;
          case kCallableIndex: printfmt("RET Callable : {}" , e.GetCallable()->name); break;
          case kVoidIndex: print("RET"); break;
          default:
            throw InternalError(fmtstr("Invalid return index : {}" , e.Index()));
        }

        return ExitCode::OK;
      } catch (const InterpreterError& e) {
        printfmt("Error at statement : {}" , stmt->ToString());
        printerr(INTERPRETER , e.what());
        return ExitCode::ERROR;
      } catch (const InternalError& e) {
        std::string msg = "Interpreter Error : " + std::string{ e.what() };
        printerr(INTERNAL , e.what());
        return ExitCode::ERROR;
      } catch (const std::exception& e) {
        printerr(INTERNAL , e.what());
        return ExitCode::ERROR;
      }
    }

    return ExitCode::OK;
  } 
      
  void Interpreter::PushCall(const std::string& name) {
    call_stack.push(name);
  }

  void Interpreter::PopCall() {
    call_stack.pop();
  }
      
  void Interpreter::Execute(Stmt* stmt , std::unique_ptr<Environment>& closure) {
    Executor emitter(this , closure);
    stmt->Accept(emitter);

    /// if we reach here the function returns void
    throw Return(std::monostate());
  }

} // namespace ylang
