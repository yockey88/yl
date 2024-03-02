#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "interpreter/interpreter.hpp"

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
#if 1
  std::string src = ReadSrc("test_files/test1.yl");
  if (src.empty()) {
    return 1;
  }

  std::cout << "[ Source ] : \n" << src << std::endl;

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

  ylang::print("AST is valid");
  ast.PrintTree();

#if 0
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
#if 1
  std::cout << "=== Execution ===" << std::endl;
  ylang::Interpreter vm(ast);
  switch (vm.Interpret()) {
    case ylang::ExitCode::OK:
      ylang::print("Interpreter exit success");
      break;
    case ylang::ExitCode::ERROR:
      ylang::printerr(ylang::ErrorType::INTERPRETER , "Interpreter exit failure");
      return 1;
  }
#else
#endif
#endif

#else
  ylang::Token a{ {} , ylang::TokenType::INTEGER , "1" };
  ylang::Token b{ {} , ylang::TokenType::INTEGER , "2" };
  ylang::Token c{ {} , ylang::TokenType::INTEGER , "3" };
  ylang::Token d{ {} , ylang::TokenType::INTEGER , "4" };

  ylang::Value val1 = ylang::Value::CreateValue(a);
  ylang::Value val2 = ylang::Value::CreateValue(b);
  ylang::Value val3 = ylang::Value::CreateValue(c);
  ylang::Value val4 = ylang::Value::CreateValue(d);

  auto env1 = std::make_unique<ylang::Environment>();
  env1->Define("a" , val1);
  env1->Define("b" , val2);

  ylang::print("=== Environment 1 ===");

  env1->PushScope("scope1");
  env1->Define("a" , val3);
  env1->Define("c" , val4);

  env1->Dump();
  
  auto env2 = ylang::Environment::CreateClosure(env1);
  ylang::print("=== Environment 2 ===");

  env2->PushScope("scope2");
  env2->Define("a" , val4);
  env2->Define("d" , val3);

  env2->Dump();

  // ylang::print(" --- After Push ---");
  // env1->Dump();

  // env1->PopScope();
  // ylang::print(" --- After Pop ---");

  // env1->Dump();


#endif

  return 0;
}
