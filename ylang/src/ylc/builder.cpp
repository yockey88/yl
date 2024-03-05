#include "ylc/builder.hpp"

#include "defines.hpp"
#include "errors.hpp"
#include "util/io.hpp"
#include "ylc/file_handlers.hpp"  
#include "preprocessor/preprocessor.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/resolver.hpp"

namespace ylang {

  IntermediateRepresentation Builder::Build() {
    verbose = args.TestFlag(flags::VERBOSE);

    Reader reader(build_file);
    std::vector<std::string> lines = reader.GetSplit('\n');

    if (verbose) {
      print(" -- Parsing build file");
    }

    for (auto& line : lines) {
      files.push_back(directory + "/" + line);
      if (verbose) {
        printfmt(" -- Adding file : {}", line);
      }
    }

    if (verbose) {
      print(" -- Processing files");
    }
    

    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    std::chrono::duration<double> elapsed;

    begin = std::chrono::steady_clock::now();
    Process();

    end = std::chrono::steady_clock::now();
    elapsed = end - begin;

    if (verbose) {
      print(" -- Finished processing files");
      printfmt(" -- Time taken : {}s", elapsed.count());
    }

    if (failed_files.size() > 0) {
      printerr(ErrorType::PREPROCESSOR, "Failed to process files");
      for (auto& f : failed_files) {
        printerr(ErrorType::PREPROCESSOR, fmtstr("  {}", f));
      }
      return IntermediateRepresentation();
    }

    begin = std::chrono::steady_clock::now();

    if (verbose) {
      print(" -- Constructing dependency graph");
    }
    ir.graph.Construct(preprocessed_srcs);
    preprocessed_srcs.clear();
    preprocessed_srcs = ir.graph.CompileOrder();

    end = std::chrono::steady_clock::now();

    elapsed = end - begin;

    if (verbose) {
      print(" -- Finished constructing dependency graph");
      printfmt(" -- Time taken : {}s", elapsed.count());
    }

    if (ir.graph.ContainsCycle()) {
      printerr(ErrorType::BUILD, "Circular dependency detected");
      return IntermediateRepresentation();
    }


    if (verbose) {
      print(" -- Tokenizing files");
    }

    begin = std::chrono::steady_clock::now();
    Tokenize();

    end = std::chrono::steady_clock::now();
    elapsed = end - begin;
    
    if (verbose) {
      print(" -- Finished tokenizing files");
      printfmt(" -- Time taken : {}s", elapsed.count());
    }

    if (failed_files.size() > 0) {
      printerr(ErrorType::LEXER, "Failed to tokenize files");
      for (auto& f : failed_files) {
        printerr(ErrorType::LEXER, fmtstr("  {}ms", f));
      }
      return IntermediateRepresentation();
    }

    if (verbose) {
      print(" -- Parsing files");
    }

    begin = std::chrono::steady_clock::now();
    Parse();

    for (auto& ast : ir.asts) {
      if (ast.IsValid()) {
        ast.PrintTree();
      }
    }

    Resolve();

    end = std::chrono::steady_clock::now();
    elapsed = end - begin;

    if (verbose) {
      print(" -- Finished parsing files");
      printfmt(" -- Time taken : {}s", elapsed.count());
    }

    if (failed_files.size() > 0) {
      printerr(ErrorType::PARSER, "Failed to parse files");
      for (auto& f : failed_files) {
        printerr(ErrorType::PARSER, fmtstr("  {}", f));
      }
      return IntermediateRepresentation();
    }

    ir.valid = true;

    return ir;
  }

  void Builder::Process() {
    auto process_file = [&](const std::string& file) {
      Reader reader(file);
      std::string src = reader.Read();

      Preprocessor preprocessor(file, src , args);
      ProcessedFile result = preprocessor.Process();
      if (result.src.empty()) {
        std::lock_guard<std::mutex> lock{ failed_files_mutex };
        failed_files.push_back(file);
        return;
      }

      std::lock_guard<std::mutex> lock(preprocessor_mutex);
      preprocessed_srcs.push_back(result);
    };


    while (files.size() > 0) {
      std::vector<std::jthread> workers;

      for (auto& f : files) {
        if (verbose) {
          printfmt(" -- Processing file : {}", f);
        }
        workers.emplace_back(process_file, f);
      }

      for (auto& w : workers) {
        w.join();
      }

      files.clear();

      for (auto& f : preprocessed_srcs) {
        for (auto& imp : f.imports) {
          if (std::find_if(preprocessed_srcs.begin(), preprocessed_srcs.end(), [&](const ProcessedFile& s) { 
            return s.name == imp; 
          }) == preprocessed_srcs.end()) {
            files.push_back(directory + "/src/" + imp + ".yl");
          }
        }
      }
    }

    for (auto& f : preprocessed_srcs) {
      if (verbose) {
        printfmt(" -- Processed file : {}", f.name);
      }
    }
  }

  void Builder::Tokenize() {
    auto lex_file = [&](const ProcessedFile& src) {
      Lexer lexer(src);
      TokenList tkns = lexer.Lex();
      if (tkns.size() == 0) {
        printerr(ErrorType::LEXER, fmtstr("Failed to tokenize file : {}", src.name));
        std::lock_guard<std::mutex> lock{ failed_files_mutex };
        failed_files.push_back(src.src);
        return;
      }

      std::lock_guard<std::mutex> lock(lexer_mutex);
      tokens.push_back(tkns);
    };

    std::vector<std::jthread> workers;

    for (auto& src : preprocessed_srcs) {
      if (verbose) {
        printfmt(" -- Tokenizing file : {}", src.name);
      }
      workers.emplace_back(lex_file, src);
    }

    for (auto& w : workers) {
      w.join();
    }
  }

  void Builder::Parse() {
    auto parse_file = [&](const TokenList& tkns) {
      Parser parser(tkns);
      Ast ast = parser.Parse();
      if (!ast.IsValid()) {
        printerr(ErrorType::PARSER, fmtstr("Failed to parse file : {}", tkns.src_name));
        std::lock_guard<std::mutex> lock{ failed_files_mutex };
        failed_files.push_back(tkns.src_name);
        return;
      }

      std::lock_guard<std::mutex> lock(parser_mutex);
      ir.asts.push_back(ast);
    };

    std::vector<std::jthread> workers;
      
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for (auto& tkns : tokens) {
      if (verbose) {
        printfmt(" -- Parsing file : {}", tkns.src_name);
      }
      workers.emplace_back(parse_file, tkns);
    }

    for (auto& w : workers) {
      w.join();
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    if (verbose) {
      printfmt(" -- Parsing took {} seconds", elapsed_seconds.count());
    }
  }

  void Builder::Resolve() {
    for (auto& ast : ir.asts) {
      Resolver resolver(ast);
      if (!resolver.Resolve()) {
        failure = true;
      }
    }
  }

} // namespace ylang
