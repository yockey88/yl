#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "lexer_tests.hpp"

TEST_F(lexer_tests1 , add_string) {
  std::cout << "[ Content ] : " << content1.src << std::endl;
  ylang::Lexer lexer(content1);
  tokens = lexer.Lex();

  EXPECT_TRUE(tokens.valid());
  EXPECT_EQ(tokens.size() , 5);

  for (const auto& token : tokens.tokens) {
    std::cout << "[" << ylang::TokenTypeStrings[token.type] << "] : " << token.value << std::endl;
  }
}

TEST_F(lexer_tests1 , add_string2) {
  std::cout << "[ Content ] : " << content1.src << std::endl;
  ylang::Lexer lexer(content1);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.size() , 5);

  ASSERT_EQ(tokens.tokens[0].type ,  ylang::TokenType::START);

  ASSERT_EQ(tokens.tokens[1].type ,  ylang::TokenType::INTEGER);
  ASSERT_EQ(tokens.tokens[1].value ,  "2") << "Expected 2 but got " << tokens.tokens[0].value;

  ASSERT_EQ(tokens.tokens[2].type ,  ylang::TokenType::PLUS);
  ASSERT_EQ(tokens.tokens[2].value ,  "+") << "Expected + but got " << tokens.tokens[1].value;

  ASSERT_EQ(tokens.tokens[3].type ,  ylang::TokenType::INTEGER);
  ASSERT_EQ(tokens.tokens[3].value ,  "2") << "Expected 2 but got " << tokens.tokens[2].value;

  ASSERT_EQ(tokens.tokens[4].type ,  ylang::TokenType::END);
}

TEST_F(lexer_tests1 , hex_string) {
  std::cout << "[ Content ] : " << content2.src << std::endl;
  ylang::Lexer lexer(content2);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.size() , 3);

  ASSERT_EQ(tokens.tokens[0].type ,  ylang::TokenType::START);

  ASSERT_EQ(tokens.tokens[1].type ,  ylang::TokenType::INTEGER);

  uint16_t value = 0x2020;
  std::stringstream ss;
  ss << std::dec << value;
  std::string str = ss.str();
  ASSERT_EQ(tokens.tokens[1].value ,  str) << "Expected " << str << " but got " << tokens.tokens[1].value;

  ASSERT_EQ(tokens.tokens[2].type ,  ylang::TokenType::END);
}

TEST_F(lexer_tests1 , float_string) {
  std::cout << "[ Content ] : " << content3.src << std::endl;
  ylang::Lexer lexer(content3);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.size() , 3);

  ASSERT_EQ(tokens.tokens[0].type ,  ylang::TokenType::START);

  ASSERT_EQ(tokens.tokens[1].type ,  ylang::TokenType::FLOAT);
  ASSERT_EQ(tokens.tokens[1].value ,  "2.3") << "Expected 2.3 but got " << tokens.tokens[1].value;

  ASSERT_EQ(tokens.tokens[2].type ,  ylang::TokenType::END);
}

TEST_F(lexer_tests1 , scientific_string) {
  std::cout << "[ Content ] : " << content4.src << std::endl;
  ylang::Lexer lexer(content4);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.size() , 3);

  ASSERT_EQ(tokens.tokens[0].type ,  ylang::TokenType::START);

  ASSERT_EQ(tokens.tokens[1].type ,  ylang::TokenType::FLOAT);
  float diff = std::stod(tokens.tokens[1].value) - 23000.0f;
  ASSERT_LT(diff , 0.0001f) 
    << "epsilon test = " << diff;

  ASSERT_EQ(tokens.tokens[2].type ,  ylang::TokenType::END);
}

TEST_F(lexer_tests1 , scientific_string2) {
  std::cout << "[ Content ] : " << content5.src << std::endl;
  ylang::Lexer lexer(content5);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.size() , 3);

  ASSERT_EQ(tokens.tokens[0].type ,  ylang::TokenType::START);

  ASSERT_EQ(tokens.tokens[1].type ,  ylang::TokenType::FLOAT);
  ASSERT_EQ(tokens.tokens[1].value ,  "0.00023") << "Expected 2.3e-4 but got " << tokens.tokens[1].value;

  ASSERT_EQ(tokens.tokens[2].type ,  ylang::TokenType::END);
}

TEST_F(lexer_tests1 , blank_string) {
  ylang::Lexer lexer(blank_content);
  tokens = lexer.Lex();

  ASSERT_TRUE(tokens.valid());
  ASSERT_EQ(tokens.size() , 0);
}

TEST_F(lexer_tests1 , error_string1) {
  std::cout << "[ Content ] : " << err_content1.src << std::endl;
  ylang::Lexer lexer(err_content1);
  tokens = lexer.Lex();

  ASSERT_FALSE(tokens.valid());
}

TEST_F(lexer_tests1 , error_string2) {
  std::cout << "[ Content ] : " << err_content2.src << std::endl;
  ylang::Lexer lexer(err_content2);
  tokens = lexer.Lex();

  ASSERT_FALSE(tokens.valid());
}

TEST_F(lexer_tests1 , error_string3) {
  std::cout << "[ Content ] : " << err_content3.src << std::endl;
  ylang::Lexer lexer(err_content3);
  tokens = lexer.Lex();

  ASSERT_FALSE(tokens.valid());
}

TEST_F(lexer_tests1 , error_string4) {
  std::cout << "[ Content ] : " << err_content4.src << std::endl;
  ylang::Lexer lexer(err_content4);
  tokens = lexer.Lex();

  ASSERT_FALSE(tokens.valid());
}
