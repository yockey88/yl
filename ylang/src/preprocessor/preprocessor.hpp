#ifndef YL_PREPROCESSOR_HPP
#define YL_PREPROCESSOR_HPP

#include <string>

namespace ylang {

  class Preprocessor {
    public:
      Preprocessor() = delete;
      ~Preprocessor() = delete;

      static std::string Preprocess(const std::string& src);
  };

};

#endif // !YL_PREPROCESSOR_HPP
