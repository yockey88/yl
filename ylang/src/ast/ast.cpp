#include "ast/ast.hpp"

#include "util/io.hpp"

namespace ylang {

  void Ast::Walk(TreeWalker& walker) {
    for (auto& node : nodes) {
      node->Accept(walker);
    }
  }
      
  void Ast::PrintTree() {
    for (auto& node : nodes) {
      printfmt(node->ToString());
    } 
  }

  std::vector<Stmt*>& Ast::Nodes() {
    return nodes;
  }

} // namespace ylang
