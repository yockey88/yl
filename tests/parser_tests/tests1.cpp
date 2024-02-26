#include "parser_tests.hpp"

#include <iostream>

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "ast/ast_expr.hpp"
#include "ast/ast_stmt.hpp"

TEST_F(parser_tests1 , literal_expr1) {
  std::cout << "[ Content ] : " << content1 << std::endl;
  ylang::Lexer lexer(content1);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  ASSERT_EQ(ast.Nodes().size() , 1);

  auto nodes = ast.Nodes();

  ASSERT_EQ(nodes[0]->type , ylang::NodeType::EXPRESSION_STMT);

  ylang::ExprStmt* expr_stmt = reinterpret_cast<ylang::ExprStmt*>(nodes[0]);

  ASSERT_NE(expr_stmt , nullptr);
  ASSERT_NE(expr_stmt->expr , nullptr);

  ylang::Expr* expr = expr_stmt->expr;

  ASSERT_EQ(expr->type , ylang::NodeType::LITERAL_EXPR);
  ASSERT_EQ(expr->ToString() , "(2)");

  ylang::LiteralExpr* literal = reinterpret_cast<ylang::LiteralExpr*>(expr_stmt->expr);

  ASSERT_NE(literal , nullptr);

  EXPECT_EQ(literal->value.type , ylang::TokenType::INTEGER);
  EXPECT_EQ(literal->value.value , "2");
}

TEST_F(parser_tests1 , binary_expr1) {
  std::cout << "[ Content ] : " << content2 << std::endl;
  ylang::Lexer lexer(content2);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  // ASSERT_NE(ast.result , nullptr);

  // ASSERT_EQ(ast.result->type , ylang::NodeType::BINARY_EXPR);
  // EXPECT_EQ(ast.result->ToString() , "((2) + (2))");

  // ylang::BinaryExpr* binary = reinterpret_cast<ylang::BinaryExpr*>(ast.result);

  // ASSERT_NE(binary , nullptr);

  // ASSERT_NE(binary->left , nullptr);
  // ASSERT_NE(binary->right , nullptr);

  // ASSERT_EQ(binary->op.type , ylang::TokenType::PLUS);

  // ASSERT_EQ(binary->left->type , ylang::NodeType::LITERAL_EXPR);
  // ASSERT_EQ(binary->right->type , ylang::NodeType::LITERAL_EXPR);

  // ylang::LiteralExpr* left = reinterpret_cast<ylang::LiteralExpr*>(binary->left);

  // ASSERT_NE(left , nullptr);
  // EXPECT_EQ(left->value.type , ylang::TokenType::INTEGER);
  // EXPECT_EQ(left->value.value , "2");

  // ylang::LiteralExpr* right = reinterpret_cast<ylang::LiteralExpr*>(binary->right);

  // ASSERT_NE(right , nullptr);
  // EXPECT_EQ(right->value.type , ylang::TokenType::INTEGER);
  // EXPECT_EQ(right->value.value , "2");
}

TEST_F(parser_tests1 , binary_expr2) {
  std::cout << "[ Content ] : " << content3 << std::endl;
  ylang::Lexer lexer(content3);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  // ASSERT_NE(ast.result , nullptr);

  // ASSERT_EQ(ast.result->type , ylang::NodeType::BINARY_EXPR);
  // EXPECT_EQ(ast.result->ToString() , "((2) * (2))");

  // ylang::BinaryExpr* binary = reinterpret_cast<ylang::BinaryExpr*>(ast.result);

  // ASSERT_NE(binary , nullptr);

  // ASSERT_NE(binary->left , nullptr);
  // ASSERT_NE(binary->right , nullptr);

  // ASSERT_EQ(binary->op.type , ylang::TokenType::STAR);

  // ASSERT_EQ(binary->left->type , ylang::NodeType::LITERAL_EXPR);
  // ASSERT_EQ(binary->right->type , ylang::NodeType::LITERAL_EXPR);

  // ylang::LiteralExpr* left = reinterpret_cast<ylang::LiteralExpr*>(binary->left);

  // ASSERT_NE(left , nullptr);
  // EXPECT_EQ(left->value.type , ylang::TokenType::INTEGER);
  // EXPECT_EQ(left->value.value , "2");

  // ylang::LiteralExpr* right = reinterpret_cast<ylang::LiteralExpr*>(binary->right);

  // ASSERT_NE(right , nullptr);
  // EXPECT_EQ(right->value.type , ylang::TokenType::INTEGER);
  // EXPECT_EQ(right->value.value , "2");
}

TEST_F(parser_tests1 , binary_expr3) {
  std::cout << "[ Content ] : " << content4 << std::endl;
  ylang::Lexer lexer(content4);
  auto tokens = lexer.Lex();

  ylang::Parser parser(tokens.tokens);
  auto ast = parser.Parse();

  // ASSERT_NE(ast.result , nullptr);

  // ASSERT_EQ(ast.result->type , ylang::NodeType::BINARY_EXPR);
  // EXPECT_EQ(ast.result->ToString() , "((2) / (2))");

  // ylang::BinaryExpr* binary = reinterpret_cast<ylang::BinaryExpr*>(ast.result);

  // ASSERT_NE(binary , nullptr);
  //   
  // ASSERT_NE(binary->left , nullptr);
  // ASSERT_NE(binary->right , nullptr);
  //   
  // ASSERT_EQ(binary->op.type , ylang::TokenType::F_SLASH);
  //   
  // ASSERT_EQ(binary->left->type , ylang::NodeType::LITERAL_EXPR);
  // ASSERT_EQ(binary->right->type , ylang::NodeType::LITERAL_EXPR);

  // ylang::LiteralExpr* left = reinterpret_cast<ylang::LiteralExpr*>(binary->left);

  // ASSERT_NE(left , nullptr);
  // EXPECT_EQ(left->value.type , ylang::TokenType::INTEGER);
  // EXPECT_EQ(left->value.value , "2");

  // ylang::LiteralExpr* right = reinterpret_cast<ylang::LiteralExpr*>(binary->right);

  // ASSERT_NE(right , nullptr);
  // EXPECT_EQ(right->value.type , ylang::TokenType::INTEGER);
  // EXPECT_EQ(right->value.value , "2");
}
