#include "ast_stmt.hpp"

namespace ylang {

  std::string ExprStmt::ToString() const {
    return fmt::format(fmt::runtime("({})") , expr->ToString());
  }

  void ExprStmt::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> ExprStmt::Emit() {
    return expr->Emit();
  }

  std::string VarDeclStmt::ToString() const {
    if (initializer) {
      return fmt::format(fmt::runtime("var {} = {}") , name.value , initializer->ToString());
    }
    return fmt::format(fmt::runtime("var {}") , name.value);
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

  std::string BlockStmt::ToString() const {
    std::string str = "{\n";
    for (auto stmt : statements) {
      str += fmt::format(fmt::runtime("{}\n") , stmt->ToString());
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

} // namespace ylang
