#ifndef YL_TOKEN_PRINTER_HPP
#define YL_TOKEN_PRINTER_HPP

#include <string_view>

#include <spdlog/fmt/fmt.h>

#include "token.hpp"

namespace ylang {
  
  constexpr std::string_view kTokenFormat = "[{}] {}";
  constexpr std::string_view kTokenFullFormat = "Token{{loc: {{index: {}, line: {}, column: {}}} type: {} value: {}}}";

  struct FmtToken {
    const Token& token;

    template <typename OS>
    OS& fmt_full(OS& os) const {
      os << fmt::format(kTokenFullFormat, 
                            token.loc.index, token.loc.line, token.loc.column,
                            TokenTypeStrings[token.type], token.value);
      return os;
    }

    template <typename OS>
    OS& fmt(OS& os) {
      os << fmt::format(kTokenFormat, TokenTypeStrings[token.type], token.value);
      return os;
    }



    constexpr FmtToken(const Token& token) : token(token) {}
  };

} // namespace ylang

#endif // !YL_TOKEN_PRINTER_HPP
