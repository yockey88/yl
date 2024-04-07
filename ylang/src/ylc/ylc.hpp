#ifndef YLC_HPP
#define YLC_HPP

#include <string>

#include "defines.hpp"

#include "ylc/arg_parser.hpp"
#include "ylc/builder.hpp"
#include "compiler/assembly.hpp"

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
  
      typedef std::pair<ExitCode , IntermediateRepresentation> BuildResult;
      typedef std::pair<ExitCode , Assembly> CompileResult;

      static ExitCode CreateProject(const Config& config , const ArgParser& args);
      static BuildResult BuildProject(const Config& config , const ArgParser& args);
      static CompileResult Compile(const IntermediateRepresentation& ir);

      static ExitCode Run(const IntermediateRepresentation& ir);
      static ExitCode Run(const Assembly& assembly);
    };

}; // namespace ylang

#endif // !YLC_HPP
