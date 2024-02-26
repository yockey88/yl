#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "compiler/bytecode_compiler.hpp"
#include "vm/vm.hpp"

std::string ReadSrc(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    return "";
  }
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

int main() {
  std::string src = ReadSrc("test_files/test1.yl");
  if (src.empty()) {
    return 1;
  }

  std::cout << "[ Source ] : " << src << std::endl;

  std::cout << "=== Tokenization ===" << std::endl;
  ylang::Lexer lexer(src);
  auto tokens = lexer.Lex();

#if 0
  for (auto token : tokens.tokens) {
    std::cout << ylang::TokenTypeStrings[token.type] << std::endl;
  }
#endif

  std::cout << "=== Parsing ===" << std::endl;
  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  if (!ast.IsValid()) {
    ylang::printerr(ylang::ErrorType::PARSER , "Invalid AST");
    return 1;
  }

  ast.PrintTree();

  std::cout << "=== Compilation ===" << std::endl;
  ylang::BytecodeCompiler compiler(ast);
  auto bytecode = compiler.Compile();

  std::cout << "=== Disassembly ===" << std::endl;
  bytecode.Dump();

  std::cout << "=== Execution ===" << std::endl;
  ylang::VM vm;
  vm.Load(&bytecode);
  // vm.Run();

  return 0;
}
