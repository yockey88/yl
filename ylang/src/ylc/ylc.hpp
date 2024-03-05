#ifndef YLC_HPP
#define YLC_HPP

#include <string>

#include "defines.hpp"

#include "ylc/arg_parser.hpp"

namespace ylang {
  
  enum Mode {
    HELP, HELP_MORE,
    BUILD_PROJ,
    RUN,
    NEW_PROJECT,
    PREPROCESS, LEX, PARSE,

    ERR,

    NUM_MODES
  }; 
  
  struct Config {
    Mode mode;
    std::string active_directory;
    std::optional<std::string> project_name;
    std::optional<std::string> build_file;
  };

  class YLC {
    public:
      YLC() {}
      ~YLC() {}
    
      static ExitCode Main(int argc, char *argv[]);
    
    private:
    
      uint64_t flags = 0;
    
      Config Configure(const ArgParser& arguments);
    
      void PrintHelp(const std::string& help_more);
      void PrintVersion();
    };

}; // namespace ylang

#endif // !YLC_HPP
