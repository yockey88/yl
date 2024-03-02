#include "ast/ast.hpp"

#include <stack>

#include "util/io.hpp"

namespace ylang {

  void Ast::Validate() {
    valid = true;
  }

  void Ast::Walk(TreeWalker& walker) {
    for (auto& node : nodes) {
      node->Accept(walker);
    }
  }
      
  void Ast::PrintTree() {
    for (auto& node : nodes) {
      print(node->ToString());
    } 
  }

  std::vector<Stmt*>& Ast::Nodes() {
    return nodes;
  }

} // namespace ylang
