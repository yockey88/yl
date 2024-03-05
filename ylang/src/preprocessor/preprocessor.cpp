#include "preprocessor/preprocessor.hpp"

#include <filesystem>

#include "defines.hpp"
#include "util/io.hpp"
#include "ylc/file_handlers.hpp"

namespace ylang {

namespace {

  bool IsWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
  }

  bool IsSpace(char c) {
    return c == ' ' || c == '\t';
  }

} // namespace (anonymous)
  
  ProcessedFile Preprocessor::Process() {
    while (CanContinue()) {
      if (Command()) {
        ConsumeCommand();
        continue;
      }

      Add();
    }

    result.name = std::filesystem::path(file).filename().stem().string();
    std::string strip_leading_ws;
    for (auto i = 0; i < result.src.size(); i++) {
      if (!IsWhitespace(result.src[i])) {
        strip_leading_ws += result.src.substr(i);
        break;
      }
    }
    result.src = strip_leading_ws;

    return result;
  }
      
  void Preprocessor::ReadFile(const std::string& file) {
    if (verbose) {
      printfmt(" -- Reading file {}", file);
    }

    Reader reader(file);
    raw_src = reader.Read();
  }

  void Preprocessor::Consume() {
    loc.index++;
  }
  
  void Preprocessor::ConsumeCommand() {
    // consume '#'
    Consume();
    
    if (Check('i')) {
      current_command += "i";
      Consume();

      if (Check('n')) { FinishImport("clude"); }
      if (Check('m')) { FinishImport("port"); }
      if (Check('f')) { If(); } 

      Consume();
    }
  }

  void Preprocessor::Add() {
    result.src += raw_src[loc.index];
    Consume();
  }
  
  void Preprocessor::Add(char c) {
    result.src += c;
  }
  
  void Preprocessor::Add(const std::string& s) {
    for (size_t i = 0; i < s.size(); i++) {
      Add(s[i]);
    }
  }

  bool Preprocessor::Command() {
    return Check('#');
  }

  bool Preprocessor::CanContinue() {
    return loc.index < raw_src.size();
  }
  
  bool Preprocessor::AtEnd() {
    return loc.index >= raw_src.size();
  }

  bool Preprocessor::Check(char c) {
    if (AtEnd()) {
      return false;
    }
    return raw_src[loc.index] == c;
  }

  bool Preprocessor::CheckNext(char c) {
    if (AtEnd() || loc.index + 1 >= raw_src.size()) {
      return false;
    }
    return raw_src[loc.index + 1] == c;
  }

  char Preprocessor::Peek() {
    if (AtEnd()) {
      return '\0';
    }
    return raw_src[loc.index];
  }
      
  void Preprocessor::FinishCommand(const std::string& expected) {
    for (size_t i = 0; i < expected.size(); i++) {
      if (!Check(expected[i])) {
        while (!IsWhitespace(Peek())) {
          current_command += Peek();
          Consume();
        }
        throw Error(fmtstr("Unknown preprocessor directive {}" , current_command) , INVALID_DIRECTIVE);
      }
      current_command += Peek();
      Consume();
    }

    current_command.clear();
  }
  
  void Preprocessor::FinishImport(const std::string& rest_of_str) {
    current_command += Peek();
    Consume();
    
    FinishCommand(rest_of_str);


    while (IsSpace(raw_src[loc.index])) {
      Consume();
    }
    
    ReadImport();
  }

  void Preprocessor::ReadImport() {
    std::string path; 
    
    char c = ';';
    if (Check('<') || Check('"')) {
      c = raw_src[loc.index];
      c = (c == '<') ? 
          '>' : '"';
      Consume();
    }

    std::string import;
    while (!Check(c)) {
      import += raw_src[loc.index];
      Consume();
    }

    Consume();
    if (c == '<' || c == '"') {
      if (!Check(';')) {
        throw Error("Expected ';' after import" , MISSING_SEMICOLON);
      }
      // semicolon after import
      Consume();
    }

    result.imports.push_back(import);
  }

  void Preprocessor::If() {
    current_command += Peek();
    Consume();

    while (IsSpace(raw_src[loc.index])) {
      Consume();
    }

    std::string condition;
    while (!IsWhitespace(Peek())) {
      condition += Peek();
      Consume();
    }
  }
      
  PreprocessorError Preprocessor::Error(const std::string& message , ErrorType type) {
    return PreprocessorError(loc, message , type);
  }

} // namespace ylang
