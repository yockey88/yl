#include "ylc/ylc.hpp"

#include "util/io.hpp"

int main(int argc , char* argv[]) {
  try {
    return ylang::YLC::Main(argc , argv);
  } catch (const std::exception& e) {
    ylang::printfmt("Error: {}", e.what());
    return ylang::ExitCode::ERROR;
  }
}
