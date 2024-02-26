#ifndef YL_DEFINES_HPP
#define YL_DEFINES_HPP

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <map>

#include <spdlog/fmt/fmt.h>

#define YL_VERSION_MAJOR 0
#define YL_VERSION_MINOR 1
#define YL_VERSION_PATCH 0
#define YL_VERSION                                                             \
  ((YL_VERSION_MAJOR << 16) | (YL_VERSION_MINOR << 8) | YL_VERSION_PATCH)

#define YL_UNUSED(x) (void)x

#define U64(c) static_cast<uint64_t>(c)

namespace ylang {

  typedef uint8_t byte;
  typedef uint16_t word;
  typedef uint32_t dword;
  typedef uint64_t qword;

  constexpr std::string_view kVersion = "0.1.0";
  constexpr std::string_view kSingleFormat = "{}";
  
  enum TokenType {
    START,

    // keywords
    BOOL , 
    CHAR , STRING ,
    SINTEGER , INTEGER ,
    I8 , I16 , I32 , I64,
    U8 , U16 , U32 , U64,
    F32 , F64,
    TRUE, FALSE,
    RETURN ,  

    // identifier
    IDENTIFIER,

    //literals 
    FLOAT,
    DOUBLE,
    ADDRESS,
    UNKNOWN,

    // operators
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACE,
    CLOSE_BRACE,
    PLUS,
    MINUS,
    STAR,
    F_SLASH,
    EQUAL,
    SEMICOLON,
    COLON,
    END,
  
    NUM_TOKEN_TYPES,
  };
  
  enum NodeType {
    LITERAL_EXPR,
    UNARY_EXPR,
    BINARY_EXPR,
    GROUPING_EXPR,
    VAR_EXPR,
  
    EXPRESSION_STMT,
    VAR_DECL_STMT,
    BLOCK_STMT,
  
    NUM_NODE_TYPES,
  };
  
  enum InstructionType : uint8_t {
    NOP,
  
    PUSH = 0x50 ,
    POP = 0x58 ,
    RET = 0xC3 ,
    CALL = 0xE8 ,
    NEG = 0xF7 ,
  
    LEA = 0x8D ,
    
    ADD = 0x01 ,
    SUB = 0x29 ,
    IMUL = 0x0F , 
    DIV = 0xF9 ,
    MOV = 0x89 ,
  
    AND = 0x21 ,
    OR = 0x09 ,
    XOR = 0x31 ,
    NOT = 0xFA ,
  
    JMP = 0xE9 ,
  
    NUM_OP_CODES,
  };
  
  enum ErrorType { 
    LEXER, 
    PARSER, 
    COMPILER, 
    RUNTIME, 
  
    NUM_ERROR_TYPES
  };
  
  enum RegisterType : uint8_t {
    RAX = 0x00 ,  
    RBX = 0x01 , 
    RCX = 0x02 , 
    RDX = 0x03 , 
    RSI = 0x04 , 
    RDI = 0x05 , 
    R8 = 0x06 , 
    R9 = 0x07 , 
    R10 = 0x08 , 
    R11 = 0x09 , 
    R12 = 0x0A , 
    R13 = 0x0B , 
    R14 = 0x0C , 
    R15 = 0x0D , 
    RBP = 0x0E , 
    RSP = 0x0F ,
    RIP = 0x10 ,
  };

  enum WordSize : uint8_t {
    BYTE = 1,
    WORD = 2,
    DWORD = 4,
    QWORD = 8 ,
    DQWORD = 16 ,

    NUM_WORD_SIZES
  };
  
  enum OperandType : uint8_t {
    IMMEDIATE ,
    REGISTER,
    DIRECT,
    INDIRECT,
    INDIRECT_OFFSET,
    INDIRECT_SCALED_INDEX,
  
    NUM_OPERAND_TYPES ,
    EMPTY = NUM_OPERAND_TYPES 
  };
  
  enum class ExitCode { OK = 0, ERR = 1 };
  
  constexpr uint8_t kBitsInByte = 8;

  constexpr uint8_t kDigitCount = 10;

  constexpr uint8_t kWriteableRegisterMax = RegisterType::RBP;
  constexpr static uint8_t kRegisterCount = RegisterType::RIP + 1;

  constexpr static uint8_t kRegisterMemorySize = QWORD * kBitsInByte;
  constexpr static uint32_t kHeapMemorySize = 0xFFFF;
  
  constexpr uint32_t kTokenTypeCount = TokenType::NUM_TOKEN_TYPES + 1;
  constexpr std::array<std::string_view, kTokenTypeCount> TokenTypeStrings = {
    "START",

    "BOOL",
    "CHAR", "STRING",
    "I8", "I16", "I32", "I64",
    "U8", "U16", "U32", "U64",
    "F32", "F64",
    "TRUE", "FALSE",
    "RETURN",

    "IDENTIFIER",

    "SINTEGER", "INTEGER",
    "FLOAT",
    "DOUBLE",
    "ADDRESS",
    "UNKNOWN",

    "OPEN_PAREN",
    "CLOSE_PAREN",
    "OPEN_BRACE",
    "CLOSE_BRACE",
    "PLUS",
    "MINUS",
    "STAR",
    "F_SLASH",
    "EQUAL",
    "SEMICOLON",
    "COLON",
    "END",
  
    "INVALID",
  };
  
  constexpr uint32_t kNodeTypeCount = NodeType::NUM_NODE_TYPES + 1;
  constexpr std::array<std::string_view, kNodeTypeCount> NodeTypeStrings = {
    "LITERAL_EXPR",
    "UNARY_EXPR",
    "BINARY_EXPR",
    "GROUPING_EXPR",
    "VAR_EXPR",
  
    "EXPRESSION_STMT",
    "VAR_DECL_STMT",
    "BLOCK_STMT",
  
    "INVALID_NODE",
  };
  
  
  constexpr uint32_t kErrorTypeCount = ErrorType::NUM_ERROR_TYPES + 1;
  constexpr std::array<std::string_view, kErrorTypeCount> ErrorTypeStrings = {
    "LEXER",
    "PARSER",
    "COMPILER",
    "RUNTIME",
  
    "INVALID_ERROR_TYPE",
  };
  
  static std::map<InstructionType , std::string> kInstructionMap = {
    {InstructionType::NOP, "NOP"},

    {InstructionType::PUSH, "PUSH"},
    {InstructionType::POP, "POP"},
    {InstructionType::RET, "RET"},
    {InstructionType::CALL, "CALL"},
    {InstructionType::NEG, "NEG"},

    {InstructionType::LEA, "LEA"},

    {InstructionType::ADD, "ADD"},
    {InstructionType::SUB, "SUB"},
    {InstructionType::IMUL, "IMUL"},
    {InstructionType::DIV, "DIV"},
    {InstructionType::MOV, "MOV"},

    {InstructionType::AND, "AND"},
    {InstructionType::OR, "OR"},
    {InstructionType::XOR, "XOR"},
    {InstructionType::NOT, "NOT"},

    {InstructionType::JMP, "JMP"},

    {InstructionType::NUM_OP_CODES, "INVALID_OP_CODE"},
  };
  
  constexpr std::array<std::string_view, kRegisterCount> RegisterStrings = {
    "RAX" , "RBX" , "RCX" , "RDX" , "RSI" , "RDI" , "RBP" , 
    "R8"  , "R9"  , "R10" , "R11" , "R12" , "R13" , "R14" , "R15" ,"RSP" , "RIP"
  };
  
  static std::map<WordSize , std::string_view> WordSizeStrings = {
    {WordSize::BYTE, "BYTE"},
    {WordSize::WORD, "WORD"},
    {WordSize::DWORD, "DWORD"},
    {WordSize::QWORD, "QWORD"},
    {WordSize::DQWORD, "DQWORD"},
    {WordSize::NUM_WORD_SIZES, "INVALID_WORD_SIZE"},
  };
  
  constexpr uint32_t kNumOperandTypes = OperandType::NUM_OPERAND_TYPES + 1;
  constexpr std::array<std::string_view, kNumOperandTypes> OperandTypeStrings = {
    "REGISTER",
    "MEMORY",
    "IMMEDIATE",
  
    "EMPTY",
  };
  
  constexpr uint32_t kOperatorCount = 11;
  constexpr std::array<char, kOperatorCount> kOperators = {
    '{' , '}' , '(' , ')' , '+', '-', '*', '/' , '=' , ';', ':'
  };
  
  struct SourceLocation {
    uint32_t index = 0;
    uint32_t line = 1;
    uint32_t column = 1;
  };

  struct address_t {
    uint64_t address;

    address_t() : address(0) {}
    address_t(uint64_t addr) : address(addr) {}
    address_t(const address_t& addr) : address(addr.address) {}
    address_t(address_t&& addr) : address(addr.address) {}

    inline address_t& operator=(const address_t& addr) {
      address = addr.address;
      return *this;
    }

    inline address_t& operator=(address_t&& addr) {
      address = addr.address;
      return *this;
    }
  };

  bool operator<(const address_t& lhs, const address_t& rhs);
  bool operator>(const address_t& lhs, const address_t& rhs);
  bool operator==(const address_t& lhs, const address_t& rhs);
  bool operator!=(const address_t& lhs, const address_t& rhs);
  bool operator<=(const address_t& lhs, const address_t& rhs);
  bool operator>=(const address_t& lhs, const address_t& rhs);

  bool operator<(const address_t& lhs, const int64_t& rhs);
  bool operator>(const address_t& lhs, const int64_t& rhs);
  bool operator==(const address_t& lhs, const int64_t& rhs);
  bool operator<(const address_t& lhs, const uint64_t& rhs);
  bool operator>(const address_t& lhs, const uint64_t& rhs);
  bool operator==(const address_t& lhs, const uint64_t& rhs);
  bool operator<(const address_t& lhs, const float& rhs);
  bool operator>(const address_t& lhs, const float& rhs);
  bool operator==(const address_t& lhs, const float& rhs);
  bool operator<(const address_t& lhs, const double& rhs);
  bool operator>(const address_t& lhs, const double& rhs);
  bool operator==(const address_t& lhs, const double& rhs);
  bool operator<(const address_t& lhs, const RegisterType& rhs);
  bool operator>(const address_t& lhs, const RegisterType& rhs);
  bool operator==(const address_t& lhs, const RegisterType& rhs);

  address_t operator+(const address_t& lhs, const address_t& rhs);
  address_t operator-(const address_t& lhs, const address_t& rhs);
  address_t operator+(const address_t& lhs, const int64_t& rhs);
  address_t operator-(const address_t& lhs, const int64_t& rhs);
  address_t operator+(const address_t& lhs, const uint64_t& rhs);
  address_t operator-(const address_t& lhs, const uint64_t& rhs);
  address_t operator+(const address_t& lhs, const RegisterType& rhs);
  address_t operator-(const address_t& lhs, const RegisterType& rhs);

  address_t operator+(const int64_t& lhs, const address_t& rhs);
  address_t operator-(const int64_t& lhs, const address_t& rhs);
  address_t operator+(const uint64_t& lhs, const address_t& rhs);
  address_t operator-(const uint64_t& lhs, const address_t& rhs);
  address_t operator+(const RegisterType& lhs, const address_t& rhs);
  address_t operator-(const RegisterType& lhs, const address_t& rhs);
  
} // namespace ylang

template <>
struct fmt::formatter<ylang::address_t> : public fmt::formatter<std::string_view> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const ylang::address_t& p, FormatContext& ctx) {
    std::string out_str = fmt::format(fmt::runtime("{0:#08x}"), p.address);
    return fmt::formatter<std::string_view>::format(out_str, ctx);
  }
};

#endif // !YL_DEFINES_HPP
