#include "lexer_tests.hpp"

#include <iostream>

#include "lexer/lexer.hpp"

TEST_F(lexer_tests2 , simple_multiplication) {
  std::cout << "[ Content ] : " << content1.src << std::endl;
  ylang::Lexer lexer(content1);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.tokens.size() , 5);

  ASSERT_EQ(tokens.tokens[0].type , ylang::TokenType::START);

  ASSERT_EQ(tokens.tokens[1].type , ylang::TokenType::INTEGER);
  ASSERT_EQ(tokens.tokens[1].value , "2");

  ASSERT_EQ(tokens.tokens[2].type , ylang::TokenType::STAR);
  ASSERT_EQ(tokens.tokens[2].value , "*");

  ASSERT_EQ(tokens.tokens[3].type , ylang::TokenType::INTEGER);
  ASSERT_EQ(tokens.tokens[3].value , "2");

  ASSERT_EQ(tokens.tokens[4].type , ylang::TokenType::END);
}
