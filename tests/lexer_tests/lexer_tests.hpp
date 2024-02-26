#ifndef YL_LEXER_TESTS_HPP
#define YL_LEXER_TESTS_HPP

#include <gtest/gtest.h>

#include "lexer/lexer.hpp"

class lexer_tests1 : public ::testing::Test {
  protected:
    std::string content1 = "2 + 2";
    std::string content2 = "0x2020";
    std::string content3 = "2.3";
    std::string content4 = "2.3e4";
    std::string content5 = "2.3e-4";
    
    std::string err_content1 = "1x0";
    std::string err_content2 = "0x";
    std::string err_content3 = "2ef";
    std::string err_content4 = "2.3.4";

    std::string blank_content = "";

    ylang::TokenList tokens;
};

class lexer_tests2 : public ::testing::Test {
  protected:
    std::string content1 = "2 * 2";
    std::string content2 = "2 / 2";
    std::string content3 = "2 - 2";
    std::string content4 = "2 + 2";
    std::string content5 = "2 + 2 - 2 * 2 / 2";
    std::string content6 = "(2 + 2) - 2 * 2 / 2";
    std::string content7 = "2 + (2 - 2) * 2 / 2";
    std::string content8 = "2 + 2 - (2 * 2) / 2";
    std::string content9 = "2 + 2 - 2 * (2 / 2)";

    ylang::TokenList tokens;
};

#endif // !YL_LEXER_TESTS_HPP
