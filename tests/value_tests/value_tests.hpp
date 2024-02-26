#ifndef YL_VALUE_TESTS_HPP
#define YL_VALUE_TESTS_HPP

#include <gtest/gtest.h>

#include "defines.hpp"

class ValueTests : public ::testing::Test {
  protected:
    ValueTests() {}
    virtual ~ValueTests() {}

    ylang::SourceLocation loc{};
};

#endif // !YL_VALUE_TESTS_HPP
