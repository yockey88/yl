#ifndef YL_LEXER_TESTS_HPP
#define YL_LEXER_TESTS_HPP

#include <gtest/gtest.h>

#include "preprocessor/preprocessor.hpp"
#include "lexer/lexer.hpp"

class lexer_tests1 : public ::testing::Test {
  protected:
    ylang::ProcessedFile content1 = { "2 + 2" , "" };
    ylang::ProcessedFile content2 = { "0x2020" , "" };
    ylang::ProcessedFile content3 = { "2.3" , "" };
    ylang::ProcessedFile content4 = { "2.3e4" , "" };
    ylang::ProcessedFile content5 = { "2.3e-4" , "" };
    
    ylang::ProcessedFile err_content1 = { "1x0" , "" };
    ylang::ProcessedFile err_content2 = { "0x" , "" };
    ylang::ProcessedFile err_content3 = { "2ef" , "" };
    ylang::ProcessedFile err_content4 = { "2.3.4" , "" };

    ylang::ProcessedFile blank_content = { "", "" };

    ylang::TokenList tokens;
};

class lexer_tests2 : public ::testing::Test {
  protected:
    ylang::ProcessedFile content1 = { "2 * 2" , "" };
    ylang::ProcessedFile content2 = { "2 / 2" , "" };
    ylang::ProcessedFile content3 = { "2 - 2" , "" };
    ylang::ProcessedFile content4 = { "2 + 2" , "" };
    ylang::ProcessedFile content5 = { "2 + 2 - 2 * 2 / 2" , "" };
    ylang::ProcessedFile content6 = { "(2 + 2) - 2 * 2 / 2" , "" };
    ylang::ProcessedFile content7 = { "2 + (2 - 2) * 2 / 2" , "" };
    ylang::ProcessedFile content8 = { "2 + 2 - (2 * 2) / 2" , "" };
    ylang::ProcessedFile content9 = { "2 + 2 - 2 * (2 / 2)" , "" };

    ylang::TokenList tokens;
};

class lexer_tests3 : public ::testing::Test {
  protected:
    ylang::ProcessedFile comment1 = { "// this is a comment" , "" };
    ylang::ProcessedFile comment2 = { "// this is a comment\n" , "" };
    ylang::ProcessedFile comment3 = { "/* this is a comment */" , "" };

    ylang::ProcessedFile comment4 = { R"(
      // this is a comment
      // this is a comment
    )" , "" };

    ylang::ProcessedFile comment5 = { R"(
      /* this is a comment */
      /* this is a comment */
    )" , "" };

    ylang::ProcessedFile comment6 = { R"(
      /* this is a comment
      this is a comment */
    )" , "" };

    ylang::TokenList tokens;
};

#endif // !YL_LEXER_TESTS_HPP
