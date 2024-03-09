#ifndef YL_SYMBOL_TABLE_HPP
#define YL_SYMBOL_TABLE_HPP

#include <string>
#include <vector>

#include "value.hpp"

namespace ylang {

  enum class SymbolType {
    STRUCT,
    FUNCTION,
  };

  struct Field {
    Value::Type type;
    std::string name;
    std::string type_name;
    size_t offset;
    size_t size;
    std::vector<Value> initial_values{ Value() };
  };

  struct Parameter {
    Value::Type type;
    std::string name;
    size_t size;
    std::vector<Value> default_values;
  };

  struct Symbol {
    std::string name;
    SymbolType type;
    size_t size; // in values (only strings and arrays > 1)

    std::vector<Field> fields;
  };

  struct FunctionSymbol {
    address_t address = 0x0;
    std::string name;
    Value::Type return_type;
    std::vector<Parameter> parameters;
    std::vector<Value> default_values;
  };

  struct DataSymbol {
    address_t address;
    size_t size;
    std::string name;
    Value::Type type;
    std::vector<Value> values;
  };

  class SymbolTable {
    public:
      SymbolTable(const std::string& asm_name)
        : name(asm_name) {}
      ~SymbolTable() {}

      Symbol& DefineStruct(const std::string& name);
      FunctionSymbol& DefineFunction(const std::string& name);
      DataSymbol& DefineVariable(const std::string& name , const std::vector<Value>& initial_values);

      void AddField(const std::string& obj_name , const std::string& name , 
                    const std::vector<Value>& initial_values);
      void AddField(Symbol& sym , const std::string& name , 
                    const std::vector<Value>& initial_values);

      Symbol* RetrieveSymbol(const std::string& name);
      size_t CalculateSize(const std::string& name) const;
      size_t CalculateSize(const Symbol* sym) const;

      bool SymbolExists(const std::string& name) const;

      void Dump() const;

      std::string TypeToString(Value::Type type) const;
      Value::Type StringToType(const std::string& type) const;
      size_t TypeSize(Value::Type type) const;
      size_t FieldSize(Symbol& sym , const std::string& field_name) const;

      const std::string& AsmName() const { return name; }

      typedef std::tuple<uint64_t , std::string_view , Value::Type> TypeHash;

      constexpr static std::array<TypeHash , Value::Type::COUNT> kTypeHashes = {
        TypeHash{ FNV("bool") , "bool" , Value::Type::BOOL } ,
        TypeHash{ FNV("char") , "char" , Value::Type::CHAR } ,
        TypeHash{ FNV("i8") , "char" , Value::Type::I8 } ,
        TypeHash{ FNV("i16") , "i16" , Value::Type::I16 } ,
        TypeHash{ FNV("i32") , "i32" , Value::Type::I32 } ,
        TypeHash{ FNV("i64") , "i64" , Value::Type::I64 } ,
        TypeHash{ FNV("u8") , "u8" , Value::Type::U8 } ,
        TypeHash{ FNV("u16") , "u16" , Value::Type::U16 } ,
        TypeHash{ FNV("u32") , "u32" , Value::Type::U32 } ,
        TypeHash{ FNV("u64") , "u64" , Value::Type::U64 } ,
        TypeHash{ FNV("f32") , "f32" , Value::Type::F32 } ,
        TypeHash{ FNV("f64") , "f64" , Value::Type::F64 } ,
        TypeHash{ FNV("address") , "address" , Value::Type::ADDRESS } ,
        TypeHash{ FNV("register") , "register" , Value::Type::REGISTER } ,
        TypeHash{ FNV("string") , "string" , Value::Type::STRING } ,

        /// there is no array , address , or register keyword so this is here as a placeholder
        TypeHash{ FNV("array") , "array" , Value::Type::ARRAY } ,
        TypeHash{ FNV("struct") , "struct" , Value::Type::STRUCT } ,
        TypeHash{ FNV("nil") , "nil" , Value::Type::NIL } ,
        TypeHash{ FNV("<anon-fn>") , "<anon-fn>" , Value::Type::CALLABLE }
      };

    private:
      std::string name;
      std::vector<Symbol> structs;
      std::map<address_t , FunctionSymbol> functions;
      std::map<address_t , DataSymbol> variables;

      address_t data_address = 0x0;
      address_t function_address = 0x0FFFFF;

      void ThrowError(const std::string& msg) const;
  };

} // namespace ylang

#endif // !YL_SYMBOL_TABLE_HPP
