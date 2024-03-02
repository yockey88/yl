#include "env/symbol_table.hpp"

#include "errors.hpp"
#include "util/io.hpp"

namespace ylang {

  Symbol& SymbolTable::DefineStruct(const std::string& name) {
    for (const auto& s : structs) {
      if (s.name == name) {
        ThrowError(fmtstr("Struct '{}' already defined" , name));
      }
    }

    structs.push_back(Symbol {
      .name = name,
      .type = SymbolType::STRUCT,
      .fields = std::vector<Field>{} 
    });

    return structs.back();
  }

  void SymbolTable::AddField(const std::string& obj_name , const std::string& field_name , const std::vector<Value>& initial_values) {
    for (auto& s : structs) {
      if (s.name == obj_name) {
        AddField(s , field_name , initial_values);
        return;
      }
    }

    ThrowError(fmtstr("Struct '{}' not defined" , obj_name));
  }
      
  void SymbolTable::AddField(Symbol& sym , const std::string& name , const std::vector<Value>& initial_values) {
    for (const auto& field : sym.fields) {
      if (field.name == name) {
        ThrowError(fmtstr("Field '{}' already defined in struct '{}'" , name , sym.name));
      }
    }

    Value::Type type = initial_values.size() > 1 ? 
      Value::Type::ARRAY : initial_values[0].type;

    sym.fields.push_back(Field {
      .type = type,
      .name = name,
      .type_name = TypeToString(type),
      .offset = sym.size,
      .size = initial_values.size(),
      .initial_values = initial_values 
    });

    sym.size += initial_values.size();
  }
      
  Symbol* SymbolTable::RetrieveSymbol(const std::string& name) {
    for (auto& s : structs) {
      if (s.name == name) {
        return &s;
      }
    }

    return nullptr;
  }

  bool SymbolTable::SymbolExists(const std::string& name) const {
    for (const auto& s : structs) {
      if (s.name == name) {
        return true;
      }
    }

    return false;
  }
      
  size_t SymbolTable::CalculateSize(const std::string& name) const {
    for (const auto& s : structs) {
      if (s.name == name) {
        return CalculateSize(&s);
      }
    }

    return 0;
  }

  size_t SymbolTable::CalculateSize(const Symbol* sym) const {
    size_t size = 0; // number of values to represent it
    for (const auto& field : sym->fields) {
      if (field.type == Value::Type::STRING || field.type == Value::Type::REGISTER) {
        ThrowError(fmtstr("Cannot calculate size of struct with string or register fields"));
      }
      size += field.size;
    }
    return size;
  }
      
  void SymbolTable::Dump() const {
    printfmt("========= Symbols ==========");
    for (const auto& s : structs) {
      printfmt("[struct] '{}'" , s.name);
      for (const auto& field : s.fields) {
        std::string field_str = fmtstr("  Field '{}' Type '{}' Size '{}'" , 
                                       field.name , field.type , field.size);
        if (field.initial_values.size() == 1) {
          field_str += fmtstr("  Initializer '{}'" , field.initial_values[0]);
        } else {
          for (const auto& val : field.initial_values) {
            field_str += fmtstr("\n  |-- Initializer '{}'" , val);
          }
        }

        printfmt(field_str);
      }
    }
    printfmt("============================");
  }      

  std::string SymbolTable::TypeToString(Value::Type type) const {
    auto& [h , n , t] = kTypeHashes[type];
    return std::string(n);
  }

  Value::Type SymbolTable::StringToType(const std::string& type) const {
    uint64_t hash = FNV(type);
    for (const auto& [h , n , t] : kTypeHashes) {
      if (h == hash) {
        return t; 
      }
    }

    ThrowError(fmtstr("Unknown type '{}' in struct" , type));

    // unreachable
    return Value::Type::NIL;
  }
      
  size_t SymbolTable::TypeSize(Value::Type type) const {
    switch (type) {
      case Value::Type::BOOL:
      case Value::Type::CHAR:
      case Value::Type::I8:
      case Value::Type::U8:
      case Value::Type::I16:
      case Value::Type::U16:
      case Value::Type::I32:
      case Value::Type::U32:
      case Value::Type::F32:
      case Value::Type::I64:
      case Value::Type::U64:
      case Value::Type::F64:
      case Value::Type::ADDRESS:
        return 1;
      case Value::Type::STRING:
        ThrowError(fmtstr("Strings within structs not supported"));
      case Value::Type::ARRAY:
        
      case Value::Type::STRUCT:
        ThrowError(fmtstr("Structs within structs not supported"));
      default:
        ThrowError(fmtstr("Unknown type '{}' in struct" , type));
    }

    // unreachable
    return 0;
  }
      
  size_t SymbolTable::FieldSize(Symbol& sym , const std::string& field_name) const {
    for (const auto& field : sym.fields) {
      if (field.name == field_name) {
        return field.size;
      }
    }

    ThrowError(fmtstr("Field '{}' not found in struct '{}'" , field_name , sym.name));

    // unreachable
    return 0;
  }

  void SymbolTable::ThrowError(const std::string& msg) const {
    if (compiling) {
      throw CompilerError(msg);
    } else {
      throw RuntimeError(msg);
    }
  }

}  // namespace ylang
