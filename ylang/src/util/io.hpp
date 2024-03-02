#ifndef YL_IO_HPP
#define YL_IO_HPP

#include <string_view>

#include "defines.hpp"

namespace ylang {

  struct Instruction;
  struct Register;

  template <typename... Args>
  void printfmt(std::string_view fmt, Args &&...args) {
    fmt::print(fmt::runtime(fmt), std::forward<Args>(args)...);
    fmt::print(std::string_view("\n"));
  }

  template <typename... Args>
  std::string fmtstr(std::string_view fmt, Args &&...args) {
    return fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...);
  }

  template <typename T>
  std::string fmtstr(std::string_view fmt , T &&val) {
    return fmt::format(fmt::runtime(fmt), std::forward<T>(val));
  }

  void print(std::string_view str);
  void printinstr(Instruction *inst);
  void printaddr(address_t addr, uint8_t *memory);
  void printreg(Register *reg);
  void printstackaddr(address_t addr, uint8_t *memory);

} // namespace ylang

#endif // !YL_IO_HPP
