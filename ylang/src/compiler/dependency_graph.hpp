#ifndef YL_DEPENDENCY_GRAPH_HPP
#define YL_DEPENDENCY_GRAPH_HPP

#include <string>
#include <vector>
#include <stack>

#include "preprocessor/preprocessor.hpp"

namespace ylang {

  struct DependencyNode {
    uint64_t fhash;
    ProcessedFile file;

    uint32_t num_dependencies = 0;
    std::vector<DependencyNode*> dependent_nodes;

    bool visited = false;
  };

  struct GraphData {
    std::vector<ProcessedFile> files;
  };

  class DependencyGraph {
    public:
      DependencyGraph() {}
      ~DependencyGraph() {}

      void Construct(const std::vector<ProcessedFile>& files);
      bool ContainsCycle() const;

      std::vector<ProcessedFile> CompileOrder() const; 

      void DumpNodes() const;
      void PrintGraph();

    private:
      std::vector<DependencyNode> nodes;
      std::vector<DependencyNode*> sorted_nodes;

      std::vector<ProcessedFile> compilation_order{};

      bool cycle = false;
      uint32_t num_edges = 0;

      void AddNode(const ProcessedFile& file);
      void ConnectNodes(const std::string& file , const std::string& dependent);
      void ConnectNodeTo(DependencyNode* node , const std::string& dependent);

      void ReverseEdges();

      std::vector<DependencyNode*> RootNodes();

      void RemoveEdge(DependencyNode* node , DependencyNode* dependent);

      void TopologicalSort();

      void GenerateCompileOrder();

      void PrintNode(const DependencyNode* node , std::stack<char>& indent_stack);

      std::string DependentsString(DependencyNode& node , std::stack<char>& indent_stack);
      void PrintDependents(DependencyNode& node , std::stack<char>& indent_stack);
  };

} // namespace ylang

#endif // !YL_DEPENDENCY_GRAPH_HPP
