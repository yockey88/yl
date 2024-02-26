#ifndef YL_COMPILER_TESTS_HPP
#define YL_COMPILER_TESTS_HPP

#include <gtest/gtest.h>

class compiler_tests1 : public ::testing::Test {
  protected:
    std::string content1 = "2 + 2";
    std::string content2 = "3 - 2";

    std::string content3 = "2 * 2";
    std::string content4 = "2 / 2";
};

#endif // !YL_COMPILER_TESTS_HPP
