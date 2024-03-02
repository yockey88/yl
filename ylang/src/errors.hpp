#ifndef YL_ERRORS_HPP
#define YL_ERRORS_HPP

#include <stdexcept>

#include "defines.hpp" 

namespace ylang {

namespace {
  
  std::string fmt_error(const SourceLocation &loc) {
    constexpr std::string_view fmt_str = " [{} , {}]";
    return fmt::format(fmt_str, loc.line, loc.column);
  }
  
} // namespace
  
  inline void printerr(ErrorType type , const std::string &message) {
    fmt::print(std::string_view("[ {} Error ] {}\n"), ErrorTypeStrings[type], message);
  }

  inline uint8_t* to_bytes(void* value) {
    return reinterpret_cast<uint8_t*>(value);
  }

  inline uint64_t to_qword(void* value) {
    return *reinterpret_cast<uint64_t*>(value);
  }

  class LexerError : public std::runtime_error {
    public:
      LexerError(SourceLocation loc, const std::string &message)
          : std::runtime_error(message + fmt_error(loc)) {}
    
      const char *what() const noexcept override {
        return std::runtime_error::what();
      }
  };
  
  class ParserError : public std::runtime_error {
    public:
      ParserError(SourceLocation loc, const std::string &message)
          : std::runtime_error(message + fmt_error(loc)) {}
    
      const char *what() const noexcept override {
        return std::runtime_error::what();
      }
  };

  class CompilerError : public std::runtime_error {
    public:
      CompilerError(const std::string &message)
          : std::runtime_error(message) {}
    
      const char *what() const noexcept override {
        return std::runtime_error::what();
      }
  };

  class RuntimeError : public std::runtime_error {
    public:
      RuntimeError(const std::string &message)
          : std::runtime_error(message) {}
    
      const char *what() const noexcept override {
        return std::runtime_error::what();
      }
  };

  class InterpreterError : public std::runtime_error {
    public:
      InterpreterError(const std::string &message)
          : std::runtime_error(message) {}
    
      const char *what() const noexcept override {
        return std::runtime_error::what();
      }
  };

  class InternalError : public std::runtime_error {
    public:
      InternalError(const std::string &message)
          : std::runtime_error(message) {}
    
      const char *what() const noexcept override {
        return std::runtime_error::what();
      }
  };

} // namespace ylang

#endif // !YL_ERRORS_HPP
