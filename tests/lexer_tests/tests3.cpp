#include "lexer_tests.hpp"

#include <iostream>

#include "lexer/lexer.hpp"

TEST_F(lexer_tests3 , comments1) {
  std::cout << "[ Content ] : " << comment1 << std::endl; 
  ylang::Lexer lexer(comment1);
  tokens = lexer.Lex();

  for (auto& token : tokens.tokens) {
    std::cout << ylang::TokenTypeStrings[token.type] << " : " << token.value << std::endl;
  }

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.tokens.size() , 2);

  ASSERT_EQ(tokens.tokens[0].type , ylang::TokenType::START);
  ASSERT_EQ(tokens.tokens[1].type , ylang::TokenType::END);
}

TEST_F(lexer_tests3 , comments2) {
  std::cout << "[ Content ] : " << comment2 << std::endl; 
  ylang::Lexer lexer(comment2);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.tokens.size() , 2);

  ASSERT_EQ(tokens.tokens[0].type , ylang::TokenType::START);
  ASSERT_EQ(tokens.tokens[1].type , ylang::TokenType::END);
}

TEST_F(lexer_tests3 , comments3) {
  std::cout << "[ Content ] : " << comment3 << std::endl; 
  ylang::Lexer lexer(comment3);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.tokens.size() , 2);

  ASSERT_EQ(tokens.tokens[0].type , ylang::TokenType::START);
  ASSERT_EQ(tokens.tokens[1].type , ylang::TokenType::END);
}

TEST_F(lexer_tests3 , comments4) {
  std::cout << "[ Content ] : " << comment4 << std::endl; 
  ylang::Lexer lexer(comment4);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.tokens.size() , 2);

  ASSERT_EQ(tokens.tokens[0].type , ylang::TokenType::START);
  ASSERT_EQ(tokens.tokens[1].type , ylang::TokenType::END);
}

TEST_F(lexer_tests3 , comments5) {
  std::cout << "[ Content ] : " << comment5 << std::endl; 
  ylang::Lexer lexer(comment5);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.tokens.size() , 2);

  ASSERT_EQ(tokens.tokens[0].type , ylang::TokenType::START);
  ASSERT_EQ(tokens.tokens[1].type , ylang::TokenType::END);
}

TEST_F(lexer_tests3 , comments6) {
  std::cout << "[ Content ] : " << comment6 << std::endl; 
  ylang::Lexer lexer(comment6);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.tokens.size() , 2);

  ASSERT_EQ(tokens.tokens[0].type , ylang::TokenType::START);
  ASSERT_EQ(tokens.tokens[1].type , ylang::TokenType::END);
}
