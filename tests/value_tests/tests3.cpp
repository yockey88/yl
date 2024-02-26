#include "value_tests.hpp"

#include "value.hpp"
#include "util/io.hpp"

TEST_F(ValueTests, negative_tests1) {
  ylang::address_t addr = 0;
  ylang::Value val = ylang::Value::CreateValue(addr);
  ASSERT_TRUE(val.IsNull());
}

