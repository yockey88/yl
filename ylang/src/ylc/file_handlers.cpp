#include "ylc/file_handlers.hpp"

#include <fstream>

#include "util/io.hpp"

namespace ylang {

  Reader::Reader(const std::filesystem::path& file_path) 
      : file_path(file_path) {}

  std::string Reader::Read() {
    if (read) {
      return contents;
    }

    std::ifstream file(file_path.string());
    if (!file.is_open()) {
      printfmt("Error: Could not open file {}", file_path.string());
      return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();

    contents = ss.str();
    read = true;

    return contents;
  }

  std::vector<std::string> Reader::GetSplit(char delim) {
    std::vector<std::string> tokens;
    std::string token;

    std::string str = Read();
    std::istringstream token_stream(str);
    while (std::getline(token_stream, token, delim)) {
      tokens.push_back(token);
    }
    return tokens;
  }

  Writer::Writer(const std::filesystem::path& file_path) 
    : file_path(file_path) {
  }

  bool Writer::Write(const std::string& data) {
    std::ofstream file(file_path.string());
    if (!file.is_open()) {
      printfmt("Error: Could not open file {}", file_path.string());
      return false;
    }

    file << data;
    return true;
  }

} // namespace ylang
