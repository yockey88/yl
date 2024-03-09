#ifndef YL_AST_HPP
#define YL_AST_HPP

#include <vector>

#include "ast/ast_base.hpp"

namespace ylang {

  class Ast {
    public:
      std::string name = "";
      std::vector<std::string> dependencies{};

      inline bool IsValid() const { return valid; }
      std::vector<Stmt*>& Nodes();

      Stmt* root = nullptr;

      void Validate();

      void Walk(TreeWalker& walker);
      void PrintTree();

    private:
      friend class Parser;

      bool valid{ false };
      std::vector<Stmt*> nodes{};
  };

} // namespace ylang

#endif // !YL_AST_HPP
