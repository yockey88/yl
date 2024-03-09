#include "compiler/bytecode_compiler.hpp"

#include "value.hpp"
#include "errors.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

namespace ylang {

  void OpEmitter::Visit(LiteralExpr& expr) {
  }

  void OpEmitter::Visit(UnaryExpr& expr) {
  }

  void OpEmitter::Visit(BinaryExpr& expr) {
  }

  void OpEmitter::Visit(CallExpr& expr) {
  }

  void OpEmitter::Visit(GroupingExpr& expr) {
  }

  void OpEmitter::Visit(VarExpr& expr) {
  }

  void OpEmitter::Visit(AssignExpr& expr) {
  }

  void OpEmitter::Visit(ArrayExpr& expr) {
  }

  void OpEmitter::Visit(ArrayAccessExpr& expr) {
  }

  void OpEmitter::Visit(ObjAccessExpr& expr) {
  }
  
  void OpEmitter::Visit(ExprStmt& stmt) {
  }

  void OpEmitter::Visit(VarDeclStmt& stmt) {
  }

  void OpEmitter::Visit(ArrayDeclStmt& stmt) {
  }

  void OpEmitter::Visit(BlockStmt& stmt) {
  }

  void OpEmitter::Visit(PrintStmt& stmt) {
  }

  void OpEmitter::Visit(IfStmt& stmt) {
  }

  void OpEmitter::Visit(WhileStmt& stmt) {
  }

  void OpEmitter::Visit(ReturnStmt& stmt) {
  }

  void OpEmitter::Visit(FunctionStmt& stmt) {
  }

  void OpEmitter::Visit(StructStmt& stmt) {
  }

  Assembly BytecodeCompiler::Compile() {
    OpEmitter emitter(&table);

    return Assembly();
  }

} // namespace ylang
