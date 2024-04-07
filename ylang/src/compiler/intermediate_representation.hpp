#ifndef YL_INTERMEDIATE_REPRESENTATION_HPP
#define YL_INTERMEDIATE_REPRESENTATION_HPP

#include <string>
#include <vector>

#include "ast/ast.hpp"
#include "env/symbol_table.hpp"
#include "compiler/dependency_graph.hpp"

namespace ylang {

  struct IntermediateRepresentation {
    bool valid = false;
    std::string project_name;

    DependencyGraph graph;
    std::vector<Ast> asts{};
    std::vector<SymbolTable> symbol_tables{};

    std::optional<SymbolTable> linked_table;
  };

} // namespace ylang

#endif // !YL_INTERMEDIATE_REPRESENTATION_HPP
