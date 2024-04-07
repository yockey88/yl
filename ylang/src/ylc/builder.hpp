#ifndef YLC_BUILDER_HPP
#define YLC_BUILDER_HPP

#include <string>
#include <vector>
#include <mutex>

#include "preprocessor/preprocessor.hpp"
#include "lexer/lexer.hpp"
#include "compiler/intermediate_representation.hpp"
#include "ylc/arg_parser.hpp"

namespace ylang {


  class Builder {
    public:
      Builder(const ArgParser& args , const std::string& directory , const std::string& build_file) 
        : args(args) , directory(directory) , build_file(build_file) {}
      ~Builder() {}

      IntermediateRepresentation Build();

    private:
      const ArgParser& args;
      std::string directory;
      std::string build_file;

      std::vector<std::string> files;
      std::vector<std::string> imported_files;

      std::vector<std::string> search_paths;

      bool verbose = false;
      bool failure = false;
    
      std::mutex preprocessor_mutex;
      std::vector<ProcessedFile> preprocessed_srcs{};
    
      std::mutex lexer_mutex;
      std::vector<TokenList> tokens{};
    
      std::mutex parser_mutex;

      std::mutex build_mutex;
    
      std::mutex failed_files_mutex;
      std::vector<std::string> failed_files{};

      IntermediateRepresentation ir;

      void Process();
      void Tokenize();
      void Parse();
  };

} // namespace ylang

#endif // !YLC_BUILDER_HPP
