#include "env/environment.hpp"

#include "errors.hpp"
#include "util/io.hpp"

namespace ylang {
  
  Heap Environment::heap;

  Environment::Environment() {
    PushScope("global");
  }

  Environment::~Environment() {
    while (current_scope != nullptr) {
      PopScope();
    }
  }
      
  void Environment::PushScope(const std::string& name) {
    Scope* new_scope = new Scope();
    Scope* temp = current_scope;

    new_scope->name = name;
    new_scope->parent = current_scope;
    current_scope = new_scope;

    if (temp != nullptr) {
      temp->child = new_scope;
    }
  }

  void Environment::PopScope() {
    Scope* old_scope = current_scope;
    current_scope = current_scope->parent;
    for (auto& callable : old_scope->callables) {
      delete callable;
    }
    delete old_scope;
  }

  address_t Environment::Define(const std::string& name , const Value& val) {
    if (current_scope == nullptr) {
      throw InternalError("No scope to define variable");
    }

    ++current_scope->stack_cursor.address;
    address_t addr = current_scope->stack_cursor;
    current_scope->variables[addr.address].name = name;
    current_scope->variables[addr.address].val = val;
    current_scope->variables[addr.address].address = addr;
    ++current_scope->num_vars;

    return addr;
  }

  address_t Environment::Define(Callable* val) {
    if (current_scope == nullptr) {
      throw InternalError("No scope to define variable");
    }

    if (val == nullptr) {
      throw InternalError("Callable is nullptr");
    }

    for (auto& callable : current_scope->callables) {
      if (callable->name == val->name) {
        throw InterpreterError(fmtstr("Callable {} already defined in scope {}" , val->name , current_scope->name));
      }
    }

    current_scope->callables.push_back(val);

    /// eventually, we will need to return an address for the callable
    return address_t{0};
  }
      
  address_t Environment::Alloc(const std::string& str) {
    if (heap.heap_cursor.address + str.size() >= kHeapMemorySize) {
      throw InternalError("Heap memory overflow");
    }

    address_t addr = heap.heap_cursor;

    for (size_t i = 0; i < str.size(); ++i) {
      char c = str[i];

      heap.variables[addr.address + i].name = str;
      heap.variables[addr.address + i].val = Value::CreateValue(c);
      heap.variables[addr.address + i].address = address_t{ addr + i };
      heap.variables[addr.address + i].total_size = str.size();
    }

    heap.num_vars += str.size();
    heap.heap_cursor.address += str.size();

    return addr;
  }
      
  address_t Environment::Alloc(Value* arr , const size_t& size , const std::string& name) {
    if (heap.heap_cursor.address + size >= kHeapMemorySize) {
      throw InternalError("Heap memory overflow");
    }

    address_t addr = heap.heap_cursor;

    for (size_t i = 0; i < size; ++i) {
      heap.variables[addr.address + i].name = name;
      heap.variables[addr.address + i].val = arr[i];
      heap.variables[addr.address + i].address = address_t{ addr + i };
      heap.variables[addr.address + i].total_size = size;
    }

    heap.num_vars += size;
    heap.heap_cursor.address += size;

    return addr;
  }
      
  address_t Environment::Alloc(Value val , const std::string& name) {
    if (heap.heap_cursor.address + 1 >= kHeapMemorySize) {
      throw InternalError("Heap memory overflow");
    }

    address_t addr = heap.heap_cursor;

    heap.variables[addr.address].name = name;
    heap.variables[addr.address].val = val;
    heap.variables[addr.address].address = addr;
    heap.variables[addr.address].total_size = 1;

    ++heap.num_vars;
    ++heap.heap_cursor.address;

    return addr;
  }

  address_t Environment::Alloc(SymbolTable& syms , const std::string& name) {
    Symbol* sym = syms.RetrieveSymbol(name);
    if (sym == nullptr) {
      throw InternalError(fmtstr("Symbol {} not found in symbol table while allocating struct" , name));
    }


    size_t struct_size = sym->size;
    if (heap.heap_cursor.address + struct_size >= kHeapMemorySize) {
      throw InterpreterError("Heap memory overflow");
    }

    address_t addr = heap.heap_cursor;

    for (size_t i = 0; i < sym->fields.size(); ++i) {
      if (sym->fields[i].type == Value::Type::ARRAY) {
        Alloc(sym->fields[i].initial_values.data() , sym->fields[i].initial_values.size() , fmtstr("{}::{}" , name , sym->fields[i].name));
      } else {
        Alloc(sym->fields[i].initial_values[0] , fmtstr("{}::{}" , name , sym->fields[i].name));
      }

    }

    heap.num_vars += struct_size;
    heap.heap_cursor.address += struct_size;

    return addr;
  }

  void Environment::Assign(const std::string& name , const Value& val) {
    if (current_scope == nullptr) {
      throw InternalError("No scope to define variable");
    }

    Scope* temp = current_scope;
    while (temp != nullptr) {
      address_t addr = 1;
      for (auto& var : temp->variables) {
        if (var.name == name) {
          var.val = val;
          return;
        }
        ++addr.address;
      }

      temp = temp->parent;
    }

    throw InterpreterError(fmtstr("Variable {} not defined" , name));
  }
      
  void Environment::Assign(const address_t& addr , const Value& val) {
    if (addr.address == 0) {
      throw InterpreterError("Null Object Access : Address 0 does not point to a variable");
    }

    if (addr.address > kHeapMemorySize) {
      throw InterpreterError(fmtstr("Address {} out of bounds" , addr.address));
    } 

    heap.variables[addr.address].val = val;
  }

  Variable& Environment::Get(const std::string& name) const {
    Scope* temp = current_scope;
    while (temp != nullptr) {
      for (auto& var : temp->variables) {
        if (var.name == name) {
          return var;
        }
      }
      temp = temp->parent;
    }

    return nil_var;
  }

  Variable& Environment::Get(const address_t& addr) const {
    if (addr.address == 0) {
      return nil_var;
    }

    if (heap.variables[addr.address].val.type != Value::Type::NIL) {
      throw InterpreterError(fmtstr("Null Object Access : Address {} does not point to a variable" , addr.address));
    }

    return heap.variables[addr.address];
  }
      
  Callable* Environment::GetCallable(const std::string& name) const {
    Scope* temp = current_scope;
    while (temp != nullptr) {
      for (auto& callable : temp->callables) {
        if (callable->name == name) {
          return callable;
        }
      }
      temp = temp->parent;
    }

    throw InterpreterError(fmtstr("Callable {} not defined" , name));
  }

  address_t Environment::GetAddress(const std::string& name) const {
    Scope* temp = current_scope;
    while (temp != nullptr) {
      for (auto& var : temp->variables) {
        if (var.name == name) {
          return var.address;
        }
      }
      temp = temp->parent;
    }

    for (auto& var : heap.variables) {
      if (var.name == name) {
        return var.address;
      }
    }

    throw InterpreterError(fmtstr("Variable {} not defined" , name));
  }
      
  Variable* Environment::Ref(const address_t& addr) {
    if (addr.address == 0) {
      throw InterpreterError("Null Object Access : Address 0 does not point to a variable");
    }

    return &heap.variables[addr.address];
  }
      
  std::string Environment::RefString(const address_t& addr) const {
    if (addr.address == 0) {
      return "nil";
    }

    if (heap.variables[addr.address].val.type == Value::Type::NIL) {
      return "nil";
    }

    std::string val;
    size_t str_length = heap.variables[addr.address].total_size;
    for (size_t i = 0; i < str_length; ++i) {
      val += heap.variables[addr.address + i].val.AsChar();
    }

    return val;
  }
      
  void Environment::Undefine(const std::string& name) {
    Scope* temp = current_scope;
    while (temp != nullptr) {
      for (auto& var : temp->variables) {
        if (var.name == name) {
          var = nil_var;
          return;
        }
      }
      
      /// we could lose the callable if we undefine it and have a memory leak
      ///   so this case is only used ot remove recursive functions from their own closure
      ///   the pointer for recursive functions is managed by the parent scope/environment
      for (auto itr = temp->callables.begin(); itr != temp->callables.end(); ++itr) {
        Callable* callable = *itr;
        if (callable->name == name) {
          temp->callables.erase(itr);
          return;
        }
      }

      temp = temp->parent;
    }

    throw InterpreterError(fmtstr("Variable {} not defined" , name));
  }

  bool Environment::Defined(const std::string& name) const {
    Scope* temp = current_scope;
    while (temp != nullptr) {
      for (auto& var : temp->variables) {
        if (var.name == name) {
          return true;
        }
      }
      for (auto& callable : temp->callables) {
        if (callable->name == name) {
          return true;
        }
      }
      temp = temp->parent;
    }

    return false;
  }

  bool Environment::IsVar(const std::string& name) const {
    Scope* temp = current_scope;
    while (temp != nullptr) {
      for (auto& var : temp->variables) {
        if (var.name == name) {
          return true;
        }
      }
      temp = temp->parent;
    }

    return false;
  }
  
  bool Environment::IsFunction(const std::string& name) const {
    Scope* temp = current_scope;
    while (temp != nullptr) {
      for (auto& var : temp->variables) {
        if (var.name == name) {
          return true;
        }
      }
      temp = temp->parent;
    }

    return false;
  }
    
  void Environment::Dump() const {
    printfmt("======= Environment ========");

    symbol_table.Dump();

    Scope* temp = current_scope;
    while (temp != nullptr) {
      printfmt("======= Scope : {}" , temp->name);

      /// <= to here because stack cursor starts at 1 
      for (uint32_t i = 0; i <= temp->num_vars; ++i) {
        Variable var = temp->variables[i];
        if (var.name != "") {
          printfmt("  [{:#08x}] {} = {}" , var.address.address , var.name , var.val);
        }
      }

      printfmt("======= Callables : {}" , temp->callables.size());
      for (auto& callable : temp->callables) {
        printfmt("    {}" , callable->name);
      }

      temp = temp->parent;
    }

    printfmt("======= Heap :");
    printfmt(" |------ Heap Cursor : {:#08x}" , heap.heap_cursor.address);
    for (uint32_t i = 0; i <= heap.num_vars; ++i) {
      Variable var = heap.variables[i];
      if (var.name != "") {
        printfmt("  [{:#08x}] {} = {}" , var.address.address , var.name , var.val);
      }
    }

    printfmt("============================");
  }
  
  std::unique_ptr<Environment> Environment::CreateClosure(std::unique_ptr<Environment>& parent) {
    std::unique_ptr<Environment> env = std::make_unique<Environment>();
    Scope* parent_temp = parent->current_scope;
    if (parent->current_scope == nullptr) {
      throw InternalError("No scope to create closure from");
    }

    /// this is to find the top level scope, when its null
    ///     we move back down one level and break
    do {
      Scope* temp = parent_temp->parent;
      if (temp == nullptr) {
        break;
      } else {
        parent_temp = temp;
      }
    } while (true);

    for (uint32_t i = 0; i <= parent_temp->num_vars; ++i) {
      Variable var = parent_temp->variables[i];
      if (var.name != "") {
        env->Define(var.name , var.val);
      }
    }

    for (auto& callable : parent_temp->callables) {
      env->Define(callable->Clone());
    }

    if (parent_temp->child == nullptr) {
      return env;
    }

    Scope* temp = parent_temp->child;
    while (temp != nullptr) {
      env->PushScope(temp->name);
      for (uint32_t i = 0; i <= temp->num_vars; ++i) {
        Variable var = temp->variables[i];
        if (var.val.type != Value::Type::NIL) {
          env->Define(var.name , var.val);
        }
      }
      for (auto& callable : temp->callables) {
        env->Define(callable->Clone());
      }
      temp = temp->child;
    }

    return env;
  }

} // namespace ylang
