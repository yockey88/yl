#include "ast_stmt.hpp"

namespace ylang {

  std::string ExprStmt::ToString() const {
    return fmtstr("({})" , expr->ToString());
  }

  void ExprStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> ExprStmt::Emit() {
    return expr->Emit();
  }

  std::string VarDeclStmt::ToString() const {
    if (initializer) {
      return fmtstr("var {} = {}", name.value , initializer->ToString());
    }
    return fmtstr("var {}", name.value);
  }

  void VarDeclStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> VarDeclStmt::Emit() {
    std::vector<Instruction> instructions;
    if (initializer) {
      instructions = initializer->Emit();
    }
    return instructions;
  }

  std::string ArrayDeclStmt::ToString() const {
    if (initializer) {
      return fmtstr("array {}[{}] = {}", name.value , size.value , initializer->ToString());
    }
    return fmtstr("array {}", name.value);
  }

  void ArrayDeclStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> ArrayDeclStmt::Emit() {
    std::vector<Instruction> instructions;
    if (initializer) {
      instructions = initializer->Emit();
    }
    return instructions;
  }

  std::string BlockStmt::ToString() const {
    std::string str = "{\n";
    for (auto stmt : statements) {
      str += fmtstr("  {}\n" , stmt->ToString());
    }
    str += "}";
    return str;
  }

  void BlockStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> BlockStmt::Emit() {
    std::vector<Instruction> instructions;
    return instructions;
  }

  std::string PrintStmt::ToString() const {
    return fmtstr("print {}" , expr->ToString());
  }

  void PrintStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> PrintStmt::Emit() {
    std::vector<Instruction> instructions;
    return instructions;
  }

  std::string IfStmt::ToString() const {
    std::string str = fmtstr("if ({}) {}" , condition->ToString() , then_branch->ToString());
    if (else_branch != nullptr) {
      str += fmtstr(" else {}" , else_branch->ToString());
    }
    return str;
  }

  void IfStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> IfStmt::Emit() {
    std::vector<Instruction> instructions;
    return instructions;
  }

  std::string WhileStmt::ToString() const {
    return fmtstr("while ({}) {}" , condition->ToString() , body->ToString());
  }

  void WhileStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> WhileStmt::Emit() {
    std::vector<Instruction> instructions;
    return instructions;
  }

  std::string ReturnStmt::ToString() const {
    if (expr == nullptr) {
      return "return";
    }
    return fmtstr("return {}" , expr->ToString());
  }

  void ReturnStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> ReturnStmt::Emit() {
    std::vector<Instruction> instructions;
    return instructions;
  }

  std::string FunctionStmt::ToString() const {
    std::string str = fmtstr("function {} (" , name.value);

    for (auto& param : params) {
      str += fmtstr("{}" , param.value);
      if (&param != &params.back()) {
        str += " , ";
      }
    }
    str += ")";
    if (body != nullptr) {
      str += " " + body->ToString();
    } else {
      str += ";";
    }

    return str;
  }

  void FunctionStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> FunctionStmt::Emit() {
    std::vector<Instruction> instructions;
    return instructions;
  }

  std::string StructStmt::ToString() const {
    std::string str = fmtstr("struct {} {{\n" , name.value);
    for (auto& field : fields) {
      str += fmtstr("  {}\n" , field->ToString());
    }
    str += "}";
    return str;
  }

  void StructStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> StructStmt::Emit() {
    std::vector<Instruction> instructions;
    return instructions;
  }

} // namespace ylang
