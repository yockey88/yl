#include "compiler_tests.hpp"

/// main for all compiler tests
int main(int argc , char* argv[]) {
  ::testing::InitGoogleTest(&argc , argv);
  return RUN_ALL_TESTS();
}
