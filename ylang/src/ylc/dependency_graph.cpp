#include "ylc/dependency_graph.hpp"

#include "defines.hpp"
#include "util/io.hpp"

namespace ylang {
      
  void DependencyGraph::Construct(const std::vector<ProcessedFile>& files) {
    for (auto& f : files) {
      AddNode(f);
    }

    for (auto& f : files) {
      for (auto& dep : f.imports) {
        ConnectNodes(dep, f.name);
      }
    }

    TopologicalSort();

    GenerateCompileOrder();
  }
  
  bool DependencyGraph::ContainsCycle() const {
    return cycle;
  }

  std::vector<ProcessedFile> DependencyGraph::CompileOrder() const {
    return compilation_order;
  }

  void DependencyGraph::DumpNodes() const {
    for (auto& node : nodes) {
      print(node.file.name);
    }
  }
  
  void DependencyGraph::PrintGraph() {
    std::stack<char> indent_stack;

    for (auto& n : nodes) {
      n.visited = false;
    }

    print(" -- Dependency Graph --");
    for (auto& node : nodes) {
      PrintDependents(node , indent_stack);
    }
  }

  void DependencyGraph::AddNode(const ProcessedFile& file) {
    DependencyNode node{};
    node.fhash = FNV(file.name);
    node.file = file;

    nodes.push_back(node);
  }

  void DependencyGraph::ConnectNodes(const std::string& file, const std::string& dependent) {
    uint64_t fhash = FNV(file);
    uint64_t dhash = FNV(dependent);
    if (fhash == dhash) {
      printerr(RECURSIVE_INCLUDE , fmtstr("File including itself : {}", file));
      return;
    }

    for (auto& node : nodes) {
      if (node.fhash == fhash) {
        ConnectNodeTo(&node , dependent);
        ++num_edges;
      }
    }
  }
      
  void DependencyGraph::ConnectNodeTo(DependencyNode* node , const std::string& dependent) {
    uint64_t dhash = FNV(dependent);

    for (auto& dep : nodes) {
      if (dep.fhash == dhash) {
        if (std::find(node->dependent_nodes.begin(), node->dependent_nodes.end(), &dep) != node->dependent_nodes.end()) {
          print(fmtstr("Duplicate include : {} -> {}", node->file.name, dep.file.name));
          return;
        }
        node->dependent_nodes.push_back(&dep);
        dep.num_dependencies++;
      }
    }
  }
      
  void DependencyGraph::ReverseEdges() {
    for (auto& node : nodes) {
      for (auto& dep : node.dependent_nodes) {
        RemoveEdge(dep, &node);
        ConnectNodeTo(dep, node.file.name);
      }
    }
  }
      
  std::vector<DependencyNode*> DependencyGraph::RootNodes() {
    std::vector<DependencyNode*> root_nodes;
    for (auto& node : nodes) {
      if (node.num_dependencies == 0) {
        root_nodes.push_back(&node);
      }
    }

    return root_nodes;
  }

      
  void DependencyGraph::RemoveEdge(DependencyNode* node , DependencyNode* dependent) {
    auto it = std::find(node->dependent_nodes.begin(), node->dependent_nodes.end(), dependent);
    if (it != node->dependent_nodes.end()) {
      dependent->num_dependencies--;
      node->dependent_nodes.erase(it);
      --num_edges;
    }
  }
      
  void DependencyGraph::TopologicalSort() {
    for (auto& node : nodes) {
      node.visited = false;
    }

    std::vector<DependencyNode*> roots = RootNodes();

    while (roots.size() > 0) {
      auto node = roots.back();
      roots.pop_back();

      node->visited = true;
      sorted_nodes.push_back(node);

      for (auto& dep : node->dependent_nodes) {
        if (dep->visited) {
          cycle = true;
          printerr(CIRCULAR_DEPENDENCY , fmtstr("Circular dependency detected" , node->file.name , dep->file.name));
          break;
        }
        RemoveEdge(node, dep);

        if (dep->num_dependencies == 0) {
          roots.push_back(dep);
        }
      }
    }

    if (num_edges > 0) {
      cycle = true;
    }
  }

  void DependencyGraph::GenerateCompileOrder() {
    if (cycle) {
      printerr(CIRCULAR_DEPENDENCY , "Circular dependency detected");
      return;
    }
    for (auto& node : sorted_nodes) {
      compilation_order.push_back(node->file);
    }
  }
      
  void DependencyGraph::PrintNode(const DependencyNode* node , std::stack<char>& indent_stack) {
    for (auto& dep : node->dependent_nodes) {
      PrintDependents(*dep , indent_stack);
    }
  }
      
  std::string DependencyGraph::DependentsString(DependencyNode& node , std::stack<char>& indent_stack) {
    std::string dep_str = fmtstr("{} ({}) [Dependents : {} | Dependencies : {}]", 
                                  node.file.name , node.fhash , node.dependent_nodes.size() , node.num_dependencies);
    if (node.visited) {
      dep_str += " (Visited)";
      return dep_str;
    }

    node.visited = true;

    if (node.dependent_nodes.size() == 0) {
      return dep_str;
    }

    dep_str += "\n";
    for (size_t i = 0; i < indent_stack.size(); ++i) {
      dep_str += "  ";
    }
    dep_str += " -> ";


    for (auto& dep : node.dependent_nodes) {
      indent_stack.push(' ');
      dep_str += DependentsString(*dep , indent_stack);
      indent_stack.pop();

      if (dep != node.dependent_nodes.back()) {
        dep_str += "\n";

        for (size_t i = 0; i < indent_stack.size(); ++i) {
          dep_str += "  ";
        }
        dep_str += " -> ";
      }
    }


    return dep_str;
  }

  void DependencyGraph::PrintDependents(DependencyNode& node , std::stack<char>& indent_stack){
    print(DependentsString(node , indent_stack));
  }

} // namespace ylang
