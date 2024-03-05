#ifndef YLC_ARG_PARSER_HPP
#define YLC_ARG_PARSER_HPP

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

namespace ylang {
   
  struct Argument {
    std::string_view flag; // short flag
    std::string_view long_flag;
    std::string_view description;
    uint64_t flag_value = 0;
    bool has_args = false;

    constexpr Argument(std::string_view flag, std::string_view lflag, std::string_view description , 
                       uint64_t flag_val , bool has_args) noexcept
      : flag(flag), long_flag(lflag), description(description) , flag_value(flag_val) , has_args(has_args) {}
  };
  
  struct ParsedArgument {
    uint64_t flag_value;
    std::string long_flag;
    std::vector<std::string> args;
  };

  bool operator==(const std::string& str, const Argument& arg);

  class ArgParser {
    public:
      ArgParser(int argc , char* argv[]) { ParseArgs(argc, argv); }

      std::string YlcExe() { return executable; }

      std::vector<std::string> Files() { return files; }

      std::string Directory() const { return active_directory; }
      std::optional<std::string> ProjectName() const { return project_name; }
      std::optional<std::string> BuildFile() const { return build_file; }
      
      std::vector<std::string> GetArgs(uint64_t flag) const;
      
      bool TestFlag(uint64_t flag) const;

    private:
      uint32_t index = 1;

      uint64_t flags = 0;
      std::vector<std::string> raw_args;

      std::unordered_map<uint64_t, ParsedArgument> parsed_args;
      
      std::string executable;
      std::string active_directory;
      std::vector<std::string> files;
      std::optional<std::string> project_name;
      std::optional<std::string> build_file;
      
      void ParseArgs(int argc, char *argv[]);
      void ProcessFlag();
      bool AtEnd();
      bool IsFile();
      void Consume();
      void ConsumeArg(const Argument& arg);
      void ConsumeArgs(const Argument& arg);
      void ConsumeFile();
      void RemoveFlag(uint64_t flag);

  };

} // namespace ylang

#endif // !YLC_ARG_PARSER_HPP
