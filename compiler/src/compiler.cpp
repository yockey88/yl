#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "compiler/bytecode_compiler.hpp"
#include "vm/vm.hpp"

int main() {
#if 1
  std::string src;
  {
    std::ifstream file("test_files/test1.yl");
    if (!file.is_open()) {
      std::cerr << "Failed to open file" << std::endl;
      return 1;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    src = ss.str();
  }

  std::cout << "[ Source ] : " << src << std::endl;

  std::cout << "=== Tokenization ===" << std::endl;
  ylang::Lexer lexer(src);
  auto tokens = lexer.Lex();

  for (auto token : tokens.tokens) {
    std::cout << ylang::TokenTypeStrings[token.type] << std::endl;
  }

  std::cout << "=== Parsing ===" << std::endl;
  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  ast.PrintTree();

  std::cout << "=== Compilation ===" << std::endl;
  ylang::BytecodeCompiler compiler(ast);
  auto bytecode = compiler.Compile();

  std::cout << "=== Disassembly ===" << std::endl;
  bytecode.Dump();

  std::cout << "=== Execution ===" << std::endl;
  ylang::VM vm;
  vm.Load(&bytecode);
  vm.Run();
#else
  try {

    ylang::Value addr_val;
    addr_val.type = ylang::Value::Type::ADDRESS;
    addr_val.value = ylang::address_t{ 0xdeadbeef };
    addr_val.size = ylang::QWORD;

    ylang::printfmt("Value Index : {}\n" , addr_val.value.index());

    ylang::address_t addr = addr_val.As<ylang::address_t>();
    ylang::printfmt("Address: {}" , addr);

  } catch (std::bad_variant_access& e) {
    fmt::print(std::string_view{ "Invalid type access into value\n" });
    fmt::print(std::string_view{ "Error: {}\n" } , e.what());
  }

#endif

  return 0;
}
