#ifndef YLC_HPP
#define YLC_HPP

#include <string>
#include <optional>

namespace ylang {

namespace flags {

  constexpr static uint64_t HELP = 1;
  constexpr static uint64_t VERSION = 1 << 1;

} // namespace flags

  struct Arg {
    std::string name;
    std::optional<std::string> value;

    Arg* next = nullptr;
  };

  struct CommandLine {
    Arg* head = nullptr;
    Arg* tail = nullptr;
  };

  class YLC {
    public:
      YLC() = delete;
      ~YLC() = delete;

      static void Main(int argc , char* argv[]);

    private:
      uint64_t flags = 0;
      
      static std::string ReadSrc(const std::string& path);

      static void PrintHelp();

      static CommandLine ParseArgs(int argc , char* argv[]);
  };

};

#endif // !YLC_HPP
