#include "ylc.hpp"

int main(int argc , char* argv[]) {
  try {
    ylang::YLC::Main(argc , argv);
  } catch (...) {
    return 1;
  }
  return 0;
}
