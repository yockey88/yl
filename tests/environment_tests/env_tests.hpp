/**
 * \file environment_tests/env_tests.hpp
 */
#ifndef YL_ENV_TESTS_HPP
#define YL_ENV_TESTS_HPP

#include <gtest/gtest.h>

#include "env/environment.hpp"

class EnvironmentTests : public ::testing::Test {
  public:
    virtual ~EnvironmentTests() override {}
};

#endif // !YL_ENV_TESTS_HPP
