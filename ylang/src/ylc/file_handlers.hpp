#ifndef YLC_FILE_HANDLERS_HPP
#define YLC_FILE_HANDLERS_HPP

#include <filesystem> 

namespace ylang {
  
  class Reader {
    public:
      Reader(const std::filesystem::path& file_path);
      ~Reader() {}

      std::string Read();
      std::vector<std::string> GetSplit(char delim);

    private:
      bool read = false;
      std::string contents;
      std::filesystem::path file_path;
  };

  class Writer {
    public:
      Writer(const std::filesystem::path& file_path); 
      ~Writer() {}

      bool Write(const std::string& data);

    private: 
      std::filesystem::path file_path;
  };

} // namespace ylang

#endif // !YLC_FILE_HANDLERS_HPP
