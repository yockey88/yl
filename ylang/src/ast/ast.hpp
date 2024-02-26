#ifndef YL_AST_HPP
#define YL_AST_HPP

#include <vector>

#include "ast/ast_base.hpp"

namespace ylang {

  class Ast {
    public:
      std::vector<Stmt*>& Nodes();

      void Walk(TreeWalker& walker);
      void PrintTree();

    private:
      friend class Parser;

      std::vector<Stmt*> nodes{};
  };

} // namespace ylang

#endif // !YL_AST_HPP
