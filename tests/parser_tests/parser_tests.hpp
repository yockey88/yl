#ifndef YL_PARSERS_TESTS_HPP
#define YL_PARSERS_TESTS_HPP

#include <gtest/gtest.h>

#include "ast/ast.hpp"

class parser_tests1 : public ::testing::Test {
  protected:
    std::string content1 = "2";
    std::string content2 = "2 + 2";

    std::string content3 = "2 * 2";
    std::string content4 = "2 / 2";

    ylang::Ast ast;
};

#endif // !YL_PARSERS_TESTS_HPP
