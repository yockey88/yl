#ifndef YL_INTERPRETER_HPP
#define YL_INTERPRETER_HPP

#include <stack>

#include "defines.hpp"
#include "tree_walker.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"
#include "env/environment.hpp"
#include "compiler/intermediate_representation.hpp"

namespace ylang {

  class Function : public Callable {
    public:
      Function(FunctionStmt& stmt , std::unique_ptr<Environment>& closure) 
          : Callable(stmt.name.value , stmt.params.size() , closure) , 
            stmt(stmt) , params(stmt.params) {}
      virtual ~Function() override {}

      virtual ReturnValue Call(Interpreter* interpreter , const std::vector<Value>& args) override final;
      virtual Callable* Clone() override final { 
        auto closure = env->CreateClosure();
        return new Function(stmt, closure);
      }

    private:
      FunctionStmt& stmt;
      std::vector<Token> params;
  };

  class Interpreter;

  class Executor : public TreeWalker {
    public:
      Executor(Interpreter* interpreter , std::unique_ptr<Environment>& env) 
          : interpreter(interpreter) , env(env) {};
      ~Executor() = default;

      void Visit(LiteralExpr& expr) override;
      void Visit(UnaryExpr& expr) override;
      void Visit(BinaryExpr& expr) override;
      void Visit(CallExpr& expr) override;
      void Visit(GroupingExpr& expr) override;
      void Visit(VarExpr& expr) override;
      void Visit(AssignExpr& expr) override;
      void Visit(ArrayExpr& expr) override;
      void Visit(ArrayAccessExpr& expr) override;
      void Visit(ObjAccessExpr& expr) override;

      void Visit(ExprStmt& stmt) override;
      void Visit(VarDeclStmt& stmt) override;
      void Visit(ArrayDeclStmt& stmt) override;
      void Visit(BlockStmt& stmt) override;
      void Visit(PrintStmt& stmt) override;
      void Visit(IfStmt& stmt) override;
      void Visit(WhileStmt& stmt) override;
      void Visit(ReturnStmt& stmt) override;
      void Visit(FunctionStmt& stmt) override;
      void Visit(StructStmt& stmt) override;

    private:
      Interpreter* interpreter;
      std::unique_ptr<Environment>& env;
      // std::unique_ptr<SymbolTable> symbol_table = std::make_unique<SymbolTable>();

      Value RAX;
      std::stack<Callable*> call_stack;
      std::stack<Callable*> callable_stack;
      std::stack<Value> value_stack;
      std::stack<Value::Type> type_stack;
      std::stack<address_t> addr_stack;
      std::stack<std::string> identifier_stack;
      std::stack<Field> field_stack;

      bool need_identifier{ false };
      bool returning{ false };

      bool CompareStrings(const Value& lhs , const Value& rhs);
      void ConcatStrings(const Value& lhs , const Value& rhs);

      bool CompareArrays(const Value& lhs , const Value& rhs);

      address_t AllocateStruct(const std::string& sym_name);
  };

  class Interpreter {
    public:
      Interpreter(const IntermediateRepresentation& interpretable) 
        : interpretable(interpretable) {}
      ~Interpreter() {}

      ExitCode Interpret();
      void PushCall(const std::string& name);
      void PopCall();
      void Execute(Stmt* stmt , std::unique_ptr<Environment>& closure);

      bool CallStackEmpty() { return call_stack.empty(); }
      std::string& TopCall() { return call_stack.top(); }

    private:
      IntermediateRepresentation interpretable;

      std::unique_ptr<Environment> global_env = std::make_unique<Environment>();

      std::stack<std::string> call_stack;
  };

} // namespace ylang

#endif // !YL_INTERPRETER_HPP
