#ifndef YL_PREPROCESSOR_HPP
#define YL_PREPROCESSOR_HPP

#include <string>
#include <vector>

#include "defines.hpp"
#include "errors.hpp"
#include "ylc/arg_parser.hpp"

namespace ylang {

  struct ProcessedFile {
    std::string src = "";
    std::string name = "";
    std::vector<std::string> imports{};
  };

  class Preprocessor {
    public:
      Preprocessor(const std::string& file_name , const std::string& raw_src , const ArgParser& args)
        : args(args) , file(file_name) ,  raw_src(raw_src) {}
    
      ProcessedFile Process();
    
    private:
      ProcessedFile result; 
      SourceLocation loc;

      const ArgParser& args;

      std::string file;
      std::string raw_src;

      std::string current_command;

      bool verbose = false;

      void ReadFile(const std::string& file);
    
      void Consume();
      void ConsumeCommand();
      void Add();
      void Add(char c);
      void Add(const std::string& s);
    
      bool Command();
      bool CanContinue();
      bool AtEnd();
      bool Check(char c);
      bool CheckNext(char c);
      char Peek();

      void FinishCommand(const std::string& expected);
    
      void FinishImport(const std::string& rest_of_str);
      void ReadImport();
    
      void If();

      PreprocessorError Error(const std::string& message , ErrorType type = ErrorType::PREPROCESSOR);
    };

}; // namespace ylang

#endif // !YL_PREPROCESSOR_HPP
