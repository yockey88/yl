#include "compiler_tests.hpp"

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "compiler/bytecode_compiler.hpp"

TEST_F(compiler_tests1 , eval_binary1) {
  std::cout << "[ Content ] : " << content1 << std::endl;
  ylang::Lexer lexer(content1);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  ylang::BytecodeCompiler compiler(ast);
  auto result = compiler.Compile();

}

TEST_F(compiler_tests1 , eval_binary2) {
  std::cout << "[ Content ] : " << content2 << std::endl;
  ylang::Lexer lexer(content2);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  ylang::BytecodeCompiler compiler(ast);
  auto result = compiler.Compile();

}

TEST_F(compiler_tests1 , eval_binary3) {
  std::cout << "[ Content ] : " << content3 << std::endl;
  ylang::Lexer lexer(content3);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  ylang::BytecodeCompiler compiler(ast);
  auto result = compiler.Compile();

}

TEST_F(compiler_tests1 , eval_binary4) {
  std::cout << "[ Content ] : " << content4 << std::endl;
  ylang::Lexer lexer(content4);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  ylang::BytecodeCompiler compiler(ast);
  auto result = compiler.Compile();

}
