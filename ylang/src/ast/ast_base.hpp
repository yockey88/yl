#ifndef YL_AST_BASE_HPP
#define YL_AST_BASE_HPP

#include <string>

#include "defines.hpp"
#include "compiler/chunk.hpp"
#include "tree_walker.hpp"

namespace ylang {

  class Node {
    public:
      NodeType type;

      Node(NodeType type) 
        : type(type) {}
      virtual ~Node() = default;
      virtual std::string ToString() const = 0;
      virtual void Accept(TreeWalker& walker) = 0;
      virtual std::vector<Instruction> Emit() = 0;

      inline NodeType GetType() const {
        return type;
      }
  };

  class Expr : public Node {
    public:
      Expr(NodeType type) 
        : Node(type) {}
      virtual ~Expr() = default;
  };

  class Stmt : public Node {
    public:
      Stmt(NodeType type) 
        : Node(type) {}
      virtual ~Stmt() = default;
  };

}

#endif // !YL_AST_BASE_HPP
