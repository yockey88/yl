#ifndef YL_PARSERS_TESTS_HPP
#define YL_PARSERS_TESTS_HPP

#include <gtest/gtest.h>

#include "preprocessor/preprocessor.hpp"
#include "ast/ast.hpp"

class parser_tests1 : public ::testing::Test {
  protected:
    ylang::ProcessedFile content1 = { "2" , "" };
    ylang::ProcessedFile content2 = { "2 + 2" , "" };

    ylang::ProcessedFile content3 = { "2 * 2" , "" };
    ylang::ProcessedFile content4 = { "2 / 2" , "" };

    ylang::Ast ast;
};

#endif // !YL_PARSERS_TESTS_HPP
