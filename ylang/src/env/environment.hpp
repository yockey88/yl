#ifndef YL_ENVIRONMENT_HPP
#define YL_ENVIRONMENT_HPP

#include <vector>

#include "defines.hpp"
#include "value.hpp"
#include "env/symbol_table.hpp"

namespace ylang {

  class Callable;
  class Interpreter;
  class Environment;
  
  struct Variable {
    std::string name;
    Value val;
    address_t address;
    size_t total_size;
  };

  static Variable nil_var{ "" , Value{}, address_t{0} };

  struct Scope {
    std::string name = "[ Empty Scope ]";

    uint32_t num_vars{ 0 };
    std::array<Variable , kStackMemorySize> variables;
    std::vector<Callable*> callables;
    Scope* parent = nullptr;
    Scope* child = nullptr;

    address_t stack_cursor{ 0 };
  };

  struct Heap {
    uint32_t anon_vars{ 0 };
    int32_t num_vars{ 0 };
    address_t heap_cursor{ 1 };
    std::array<Variable , kHeapMemorySize> variables;
  };

  class Environment {
    public:
      Environment();
      ~Environment();

      void PushScope(const std::string& name = "[ Empty Scope ]");
      void PopScope();

      address_t Define(const std::string& name , const Value& val);
      address_t Define(Callable* val);

      address_t Alloc(const std::string& str);
      address_t Alloc(Value* arr , const size_t& size , const std::string& name);
      address_t Alloc(Value val , const std::string& name);
      address_t Alloc(SymbolTable& syms , const std::string& name);

      void Assign(const std::string& name , const Value& val);
      void Assign(const address_t& addr , const Value& val);
      Variable& Get(const std::string& name) const;
      Variable& Get(const address_t& addr) const;
      Callable* GetCallable(const std::string& name) const;
      address_t GetAddress(const std::string& name) const;

      // Value* Ref(const std::string& name);
      Variable* Ref(const address_t& name);
      std::string RefString(const address_t& addr) const;

      Scope* GetCurrentScope() { return current_scope; }
      Heap& GetHeap() { return heap; }
      SymbolTable& Symbols() { return symbol_table; }

      void Undefine(const std::string& name);

      bool Defined(const std::string& name) const;
      bool IsVar(const std::string& name) const;
      bool IsFunction(const std::string& name) const;

      void Dump() const;

      std::unique_ptr<Environment> CreateClosure();

    private:
      Scope* current_scope = nullptr;
      static Heap heap;

      SymbolTable symbol_table = SymbolTable("<YL-DYNAMIC-ENVIRONMENT>");
  };
 
  constexpr uint32_t kValueIndex = 0;
  constexpr uint32_t kCallableIndex = 1;
  constexpr uint32_t kVoidIndex = 2;

  typedef std::variant<Value , Callable* , std::monostate> ReturnValue;

  class Return : public std::exception {
    public:
      Return(ReturnValue value = std::monostate{}) 
        : value(value) {}
      ~Return() {}

      ReturnValue GetReturn() const { return value; }

      Value GetValue() const { return std::get<Value>(value); }
      Callable* GetCallable() const { return std::get<Callable*>(value); }

      uint32_t Index() const { return value.index(); }

      bool IsValue() const { return value.index() == kValueIndex; }
      bool IsCallable() const { return value.index() == kCallableIndex; }
      bool IsVoid() const { return value.index() == kVoidIndex; }

    private:
      ReturnValue value;
  };
  
  class Callable {
    public:
      std::string name;
      uint32_t arity;
      address_t address;

      std::unique_ptr<Environment> env;

      Callable(const std::string& name , uint32_t arity , std::unique_ptr<Environment>& closure) 
          : name(name) , arity(arity) , env(std::move(closure)) {}
      virtual ~Callable() {}

      virtual ReturnValue Call(Interpreter* interpreter , const std::vector<Value>& args) = 0;
      virtual Callable* Clone() = 0; 
  };

} // namespace ylang

#endif // !YL_ENVIRONMENT_HPP
