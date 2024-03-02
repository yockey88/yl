#include "ast/ast_expr.hpp"

#include <spdlog/fmt/fmt.h>

namespace ylang {

  std::string LiteralExpr::ToString() const {
    constexpr std::string_view fmt = "({})";
    return fmtstr(fmt , value.value);
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
    return fmtstr(fmt , op.value , right->ToString());
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
    return fmtstr(fmt , left->ToString(), op.value , right->ToString());
  }

  void BinaryExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> BinaryExpr::Emit() {
    return {};
  }

  CallExpr::~CallExpr() {
    delete callee;
    for (auto arg : args) {
      delete arg;
    }
  }

  std::string CallExpr::ToString() const {
    std::string str = fmtstr("({} (" , callee->ToString());
    for (auto arg : args) {
      str += fmtstr("{}" , arg->ToString());
      if (arg != args.back()) {
        str += " , ";
      }
    }
    str += "))";
    return str;
  }

  void CallExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> CallExpr::Emit() {
    return {};
  }

  GroupingExpr::~GroupingExpr() {
    delete expr;
  }

  std::string GroupingExpr::ToString() const {
    constexpr std::string_view fmt = "({})";
    return fmtstr(fmt , expr->ToString());
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
    return fmtstr(fmt , name.value);
  }

  void VarExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> VarExpr::Emit() {
    return {};
  }

  AssignExpr::~AssignExpr() {
    delete value;
  }

  std::string AssignExpr::ToString() const {
    constexpr std::string_view fmt = "({} = {})";
    return fmtstr(fmt , name.value , value->ToString());
  }

  void AssignExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> AssignExpr::Emit() {
    return {};
  }

  ArrayExpr::~ArrayExpr() {
    for (auto element : elements) {
      delete element;
    }
  }

  std::string ArrayExpr::ToString() const {
    std::string str = fmtstr("([");
    for (auto& element : elements) {
      str += fmtstr("{}" , element->ToString());
      if (element != elements.back()) {
        str += " , ";
      }
    }
    str += "])";
    return str;
  }

  void ArrayExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> ArrayExpr::Emit() {
    return {};
  }

  ArrayAccessExpr::~ArrayAccessExpr() {
    delete array;
    delete index;
  }

  std::string ArrayAccessExpr::ToString() const {
    constexpr std::string_view fmt = "({}[{}])";
    return fmtstr(fmt , array->ToString() , index->ToString());
  }

  void ArrayAccessExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> ArrayAccessExpr::Emit() {
    return {};
  }

  std::string ObjAccessExpr::ToString() const {
    constexpr std::string_view fmt = "({}.{})";
    constexpr std::string_view fmtassign = "({}.{} = {})";
    if (assignment != nullptr) {
      return fmtstr(fmtassign , obj->ToString() , member.value , assignment->ToString());
    }
    return fmtstr(fmt , obj->ToString() , member.value);
  } 

  void ObjAccessExpr::Accept(TreeWalker& walker) {
    walker.Visit(*this);
  }

  std::vector<Instruction> ObjAccessExpr::Emit() {
    return {};
  }

} // namespace ylang
