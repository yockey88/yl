#include "ylc.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "preprocessor/preprocessor.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "interpreter/interpreter.hpp"

#include "defines.hpp"

namespace ylang {

  void YLC::Main(int argc , char* argv[]) {
    auto args = ParseArgs(argc , argv);

    std::string src = ReadSrc("test_files/test1.yl");
    if (src.empty()) {
      return;
    }

    std::cout << "[ Source ] : \n" << src << std::endl;

#if 1 // preprocessor switch
    std::cout << "=== Preprocessing ===" << std::endl;
    std::string new_src = ylang::Preprocessor::Preprocess(src);
#endif

#if 1 // lexer switch 
    std::cout << "=== Tokenization ===" << std::endl;
    ylang::Lexer lexer(src);
    auto tokens = lexer.Lex();

#if 0 // debug (print tokens) switch
    for (auto token : tokens.tokens) {
      std::cout << ylang::TokenTypeStrings[token.type] << std::endl;
    }
#endif
#endif

#if 0 // parser switch
    std::cout << "=== Parsing ===" << std::endl;
    ylang::Parser parser(tokens.tokens);
    auto ast = parser.Parse();

    if (!ast.IsValid()) {
      ylang::printerr(ylang::ErrorType::PARSER , "Invalid AST");
      return 1;
    }

    ylang::print("AST is valid");
    ast.PrintTree();
#endif

#if 0 // compiler switch
    std::cout << "=== Compilation ===" << std::endl;
    ylang::BytecodeCompiler compiler(ast);
    auto bytecode = compiler.Compile();

    std::cout << "=== Disassembly ===" << std::endl;
    bytecode.Dump();

    std::cout << "=== Execution ===" << std::endl;
    ylang::VM vm;
    vm.Load(&bytecode);
    vm.Run();
#endif

#if 0 // interpreter switch
    std::cout << "=== Execution ===" << std::endl;
    ylang::Interpreter vm(ast);
    switch (vm.Interpret()) {
      case ylang::ExitCode::OK:
        ylang::print("Interpreter exit success");
        break;
      case ylang::ExitCode::ERROR:
        ylang::printerr(ylang::ErrorType::INTERPRETER , "Interpreter exit failure");
        return 1;
    }
#endif

#if 1 // testing/debugging switch
    std::cout << "=== Testing ===" << std::endl;
#endif
  }

  std::string YLC::ReadSrc(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
      std::cerr << "Failed to open file" << std::endl;
      return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
  }

  void YLC::PrintHelp() {
    constexpr std::string_view help = R"(
      Usage: ylc [options] <input> -o <output>
      Options:
        -h, --help      Print this help message
        -v, --version   Print version information
    \n)";
    fmt::print(help);
  }

  CommandLine YLC::ParseArgs(int argc , char* argv[]) {
    CommandLine cmd;
    Arg* arg = nullptr;

    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (arg != nullptr) {
          cmd.tail->next = arg;
        }
        arg = new Arg();
        arg->name = argv[i];
        cmd.tail = arg;
      } else {
        if (arg == nullptr) {
          arg = new Arg();
          arg->name = argv[i];
          cmd.tail = arg;
        } else {
          arg->value = argv[i];
        }
      }
    }

    return cmd;
  }

} // namespace ylang
