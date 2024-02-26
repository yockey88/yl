#include "ast/ast_expr.hpp"

#include <spdlog/fmt/fmt.h>

namespace ylang {

  std::string LiteralExpr::ToString() const {
    constexpr std::string_view fmt = "({})";
    return fmt::format(fmt::runtime(fmt) , value.value);
  }

  void LiteralExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> LiteralExpr::Emit() {
    return {};
  }

  UnaryExpr::~UnaryExpr() {
    delete right;
  }

  std::string UnaryExpr::ToString() const {
    constexpr std::string_view fmt = "({} {})";
    return fmt::format(fmt::runtime(fmt) , op.value , right->ToString());
  }

  void UnaryExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> UnaryExpr::Emit() {
    return {};
  }

  BinaryExpr::~BinaryExpr() {
    delete left;
    delete right;
  }
  
  std::string BinaryExpr::ToString() const {
    constexpr std::string_view fmt = "({} {} {})";
    return fmt::format(fmt::runtime(fmt) , left->ToString(), op.value , right->ToString());
  }

  void BinaryExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> BinaryExpr::Emit() {
    return {};
  }

  GroupingExpr::~GroupingExpr() {
    delete expr;
  }

  std::string GroupingExpr::ToString() const {
    constexpr std::string_view fmt = "({})";
    return fmt::format(fmt::runtime(fmt) , expr->ToString());
  }

  void GroupingExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> GroupingExpr::Emit() {
    return expr->Emit();
  }

  VarExpr::~VarExpr() {}

  std::string VarExpr::ToString() const {
    constexpr std::string_view fmt = "({})";
    return fmt::format(fmt::runtime(fmt) , name.value);
  }

  void VarExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> VarExpr::Emit() {
    return {};
  }

} // namespace ylang
