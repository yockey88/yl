#include "ylc/arg_parser.hpp"

#include <stdexcept>
#include <filesystem>

#include "errors.hpp"
#include "util/io.hpp"

namespace ylang {

  bool operator==(const std::string& str, const Argument& arg) {  
    if (arg.flag.empty()) {
      return str == arg.long_flag;
    }
    return str == arg.flag;
  }

  std::vector<std::string> ArgParser::GetArgs(uint64_t flag) const {
    if (parsed_args.find(flag) == parsed_args.end()) {
      return {};
    }

    return parsed_args.at(flag).args;
  }

  void ArgParser::ParseArgs(int argc, char *argv[]) {
    if (argc < 2) {
      flags |= flags::HELP;
      return;
    }
  
    for (int i = 0; i < argc; i++) {
      raw_args.push_back(argv[i]);
      if (raw_args.back().size() < 2) {
        throw std::runtime_error(fmtstr("Invalid argument : {}", raw_args.back()));
      }
    }
  
    executable = raw_args[0];

    while (!AtEnd()) {
      ProcessFlag();
    }

    if (TestFlag(flags::CREATE_PROJECT) && TestFlag(flags::BUILD)) {
      print(" -- Disregarding build command");
    }

    if (TestFlag(flags::CREATE_PROJECT) && TestFlag(flags::RUN)) {
      print(" -- Disregarding run command");
    }

    if (TestFlag(flags::BUILD) && TestFlag(flags::RUN)) {
      RemoveFlag(flags::BUILD); 
    }

    if (TestFlag(flags::HELP) && TestFlag(flags::HELP_MORE)) {
      RemoveFlag(flags::HELP);
    }

    if (TestFlag(flags::CREATE_PROJECT)) {
      if (GetArgs(flags::CREATE_PROJECT).size() != 1) {
        throw std::runtime_error("'new' command requires project name argument");
      } else {
        active_directory = GetArgs(flags::CREATE_PROJECT)[0];
      }
    }

    if (TestFlag(flags::BUILD)) {
      if (GetArgs(flags::BUILD).size() != 1) {
        active_directory = ".";
      } else {
        active_directory = GetArgs(flags::BUILD)[0];
      }
    }
  }
  
  bool ArgParser::TestFlag(uint64_t flag) const {
    return flags & flag;
  }
  
  void ArgParser::ProcessFlag() {
    std::string flag = raw_args[index];
    if (flag.empty()) {
      Consume();
      return;
    }

    for (const auto& arg : kArguments) {
      if (flag == arg || flag == arg.long_flag) {
        if (arg.has_args) {
          ConsumeArgs(arg);
        } else {
          ConsumeArg(arg);
        }
        return;
      } 
    }
  
    throw std::runtime_error(fmtstr("Invalid flag : {}", flag));
  }
  
  bool ArgParser::AtEnd() {
    if (index >= raw_args.size()) {
      return true;
    }
    return false;
  }
  
  bool ArgParser::IsFile() {
    if (AtEnd()) {
      return false;
    }
    return std::filesystem::exists(raw_args[index]);
  }
  
  void ArgParser::Consume() {
    index++; 
  }
  
  void ArgParser::ConsumeArg(const Argument& arg) {
    parsed_args[arg.flag_value] = ParsedArgument{
      .flag_value = arg.flag_value,
      .long_flag = std::string{ arg.long_flag },
      .args = {}
    };

    flags |= arg.flag_value;
    Consume();
  }

  void ArgParser::ConsumeArgs(const Argument& arg) {
    auto& parg = parsed_args[arg.flag_value] = ParsedArgument{
      .flag_value = arg.flag_value,
      .long_flag = std::string{ arg.long_flag } ,
      .args = {}
    };

    flags |= arg.flag_value;
    Consume();

    while (!AtEnd() && raw_args[index][0] != '-') {
      parg.args.push_back(raw_args[index]);
      Consume();
    }
  }

  void ArgParser::ConsumeFile() {
    if (IsFile()) {
      files.push_back(raw_args[index]);
      Consume();
    } else {
      printerr(FILE_IO , fmtstr("File does not exist : {}", raw_args[index]));
      Consume();
    }
  }

  void ArgParser::RemoveFlag(uint64_t flag) {
    flags &= ~flag;
  }


} // namespace ylang

