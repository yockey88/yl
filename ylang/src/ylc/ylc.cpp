#include "ylc.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include <spdlog/fmt/fmt.h>

#include "util/io.hpp"
#include "ylc/builder.hpp"

namespace ylang {

namespace /* detail */ {

namespace version {

constexpr static uint32_t MAJOR = 0;
constexpr static uint32_t MINOR = 1;
constexpr static uint32_t PATCH = 0;
constexpr static std::string_view kVersionFmtStr = "Y-Lang Compiler v{}.{}.{}";

}; // namespace version

namespace help {

constexpr static std::string_view usage = "Usage: ylc [files | project file] [options]";
constexpr std::string_view options =
    "Options: \n"
    "-h, --help                           Print this help message\n"
    "-hm [feature-name], --help-more      Print more detailed help message\n"
    "-v, --version                        Print version information\n"
    "-V, --verbose                        Enable verbose mode\n"
    "\n"
    "[ File Compilation (--help-more file) ]\n"
    "-o, --output [mode]                  Outputs compiled bytecode to file <input-file-stem>.ylx)\n"
    "-I, --include [directory-list]       Add include directories to search path\n"
    "\n"
    "[ Project Management (--help-more project) ]\n"
    "new <project-name> [options]         Create a new project\n"
    "                   -d, --directory   Project directory\n"
    "                   -f, --force       Overwrite existing project\n"
    "build [project-name] [options]       Build project\n"
    "run [project-name] [options]         Run project\n"
    "\n";

constexpr std::string_view detailed_help = 
  "Detailed help\n"
  "-pp, --preprocess                      Output preprocessesed source code (<input-file-stem>.ylp)\n"
  "-l, --lex                              Lexes the input file (<inpt-file-stem>.yllt)\n"
  "-p, --parse                            Parses the input file (<>.ylast)\n";

} // namespace help
  
  static ExitCode CreateProject(const Config& config , const ArgParser& args) {
    if (!config.project_name.has_value()) {
      printerr(ErrorType::INVALID_ARGS , "No project name provided");
      return ExitCode::ERROR;
    }

    bool verbose = args.TestFlag(flags::VERBOSE);
    if (verbose) {
      printfmt(" -- Creating project : {}", config.project_name.value());
    }

    std::string project_name = config.project_name.value();
    std::string proj_dir = args.TestFlag(flags::DIRECTORY) ? 
      args.GetArgs(flags::DIRECTORY).front() : project_name;

    if (verbose) {
      printfmt(" -- Project configuration : {}/{}", proj_dir, project_name);
    }

    if (std::filesystem::exists(proj_dir)) {
      if (verbose) {
        print(" -- Project already exists");
      }

      if (args.TestFlag(flags::FORCE)) {
        if (verbose) {
          print(" -- Forcing project creation");
        }
        std::filesystem::remove_all(proj_dir);
      } else {
        printerr(ErrorType::FILE_IO, "Project already exists");
        return ExitCode::ERROR;
      }
    }

    if (verbose) {
      printfmt(" -- Creating project directory : {}", proj_dir);
      printfmt(" -- Creating project source directory : {}", proj_dir + "/src");
    }
    std::filesystem::create_directory(proj_dir);
    std::filesystem::create_directory(proj_dir + "/src");

    if (verbose) {
      print(" -- Creating build file");
    }
    std::ofstream build_file(proj_dir + "/" + project_name  + ".ylbld");
    if (!build_file.is_open()) {
      printerr(ErrorType::FILE_IO, "Failed to create build file");
      return ExitCode::ERROR;
    }


    return ExitCode::OK;
  }

  typedef std::pair<ExitCode , IntermediateRepresentation> BuildResult;

  static BuildResult BuildProject(const Config& config , const ArgParser& args) {
    bool verbose = args.TestFlag(flags::VERBOSE);
    if (verbose) {
      print(" -- Building project");
    }

    if (!config.project_name.has_value()) {
      printerr(ErrorType::INVALID_ARGS, "No project name provided");
      return { ExitCode::ERROR , IntermediateRepresentation() };
    }

    std::string project_name = config.project_name.value();
    std::string directory = args.TestFlag(flags::DIRECTORY) ? 
      args.GetArgs(flags::DIRECTORY).front() : project_name;

    std::string build_file = directory + "/" + project_name + ".ylbld";
    if (!std::filesystem::exists(build_file)) {
      printerr(ErrorType::FILE_IO, fmtstr("Project at path {} does not exist" , build_file));
      return { ExitCode::ERROR , IntermediateRepresentation() };
    }

    if (verbose) {
      printfmt(" -- Build file : {}", build_file);
    }

    if (!std::filesystem::exists(build_file)) {
      printerr(ErrorType::FILE_IO, "Build file does not exist");
      return { ExitCode::ERROR , IntermediateRepresentation() };
    }

    if (verbose) {
      print(" -- Building project");
    }

    Builder builder(args , directory , build_file);
    auto ir = builder.Build();

    if (!ir.valid) {
      printerr(ErrorType::COMPILER, "Failed to build project");
      return { ExitCode::ERROR , IntermediateRepresentation() };
    }

    if (verbose) {
      print(" -- Intermediate Representation constructed");
    }

    print("=== Project built successfully ===");
    
    return { ExitCode::OK , ir };
  }

} // namespace <detail>

  ExitCode YLC::Main(int argc, char *argv[]) {
    print("=== YLC ===");
    std::filesystem::current_path("C:/Yock/code/ylsl");
    ArgParser args(argc, argv);

    YLC ylc;
    Config config = ylc.Configure(args);

    if (args.TestFlag(flags::VERSION)) {
      ylc.PrintVersion();
    }

    ExitCode exit = ExitCode::OK;
    switch (config.mode) {
      case Mode::HELP: 
        ylc.PrintHelp(""); 
        return ExitCode::OK;
      case Mode::HELP_MORE: 
        // active_directory stores the argument for --help-more
        ylc.PrintHelp(config.active_directory);
        return ExitCode::OK;
      case Mode::NEW_PROJECT: 
        exit = CreateProject(config , args);
      break;
      case Mode::BUILD_PROJ: {
        auto [exit , ir] = BuildProject(config , args);
        if (exit == ExitCode::ERROR) {
          break;
        }
      } break;
      case Mode::RUN:
        throw std::runtime_error("run command not implemented");
        break;
      case Mode::ERR:
        printerr(ErrorType::INVALID_ARGS, "Invalid arguments");
      default:
        return ExitCode::ERROR;
    }

    switch (exit) {
      case ExitCode::ERROR:
        printerr(ErrorType::COMPILER, "Failed to build project");
      case ExitCode::OK:
        return exit;
      default:
        throw std::runtime_error("Unknown Error");
        break;
    }
  }
  
  Config YLC::Configure(const ArgParser& arguments) {
    if (arguments.TestFlag(flags::DEBUG) || arguments.TestFlag(flags::VERBOSE)) {
      print("============== VERBOSE >");
    }

    if (arguments.TestFlag(flags::HELP)) {
      return Config{ Mode::HELP, "", std::nullopt, std::nullopt };
    }

    if (arguments.TestFlag(flags::HELP_MORE)) {
      auto args = arguments.GetArgs(flags::HELP_MORE);
      if (args.size() != 1) {
        printerr(ErrorType::INVALID_ARGS, "No argument provided for --help-more");
        return Config{ Mode::ERR, "", std::nullopt, std::nullopt };
      }

      return Config{ Mode::HELP_MORE, args[0], std::nullopt, std::nullopt };
    }
  
    if (arguments.TestFlag(flags::CREATE_PROJECT)) {
      Config config;
      config.mode = Mode::NEW_PROJECT;

      auto args = arguments.GetArgs(flags::CREATE_PROJECT);
      if (args.size() != 1 || args[0].empty()) {
        printerr(ErrorType::INVALID_ARGS, "No project name provided for 'new'");
        return Config{ Mode::ERR, "", std::nullopt, std::nullopt };
      }

      config.project_name = args[0];
      config.active_directory = arguments.Directory();

      return config;
    }

    if (arguments.TestFlag(flags::BUILD) || arguments.TestFlag(flags::RUN)) {
      uint64_t flag = arguments.TestFlag(flags::BUILD) ? 
        flags::BUILD : flags::RUN;
      
      Config config;
      config.mode = arguments.TestFlag(flags::BUILD) ? 
        Mode::BUILD_PROJ : Mode::RUN;

      auto args = arguments.GetArgs(flag);

      if (args.size() != 1 || args[0].empty()) {
        std::string flag_str = arguments.TestFlag(flags::BUILD) ? 
          "build" : "run";
        printerr(ErrorType::INVALID_ARGS, fmtstr("Invalid number of arguments for '{}'" , flag_str));
        return Config{ Mode::ERR, "", std::nullopt, std::nullopt };
      }
      
      config.project_name = args[0];
      config.active_directory = arguments.Directory();

      return config;
    }

    return Config{ Mode::ERR, "", std::nullopt, std::nullopt };
  }
  
  
  void YLC::PrintHelp(const std::string& help_more) {
    if (help_more.empty()) {
      print(help::usage);
      print(help::options);
      return;
    }

    /// temporary
    if (help_more == "file") {
      print(help::detailed_help);
      return;
    } else if (help_more == "project") {
      print(help::detailed_help);
      return;
    }

    throw std::runtime_error("Unknown argument for --help-more");
  }
  
  void YLC::PrintVersion() {
    printfmt(version::kVersionFmtStr, version::MAJOR, version::MINOR,
             version::PATCH);
  }

} // namespace ylang
