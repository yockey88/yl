#ifndef YL_REGISTER_HPP
#define YL_REGISTER_HPP

#include "defines.hpp"
#include "value.hpp"

namespace ylang {

#pragma pack(push , 1)

  struct Register {
    RegisterType type;

    WordSize Size();
    Value::Type ValueType();

    uint8_t* AsBytes();

    int8_t AsSByte();
    uint8_t AsByte();

    int16_t AsSWord();
    uint16_t AsWord();

    int32_t AsSDWord();
    uint32_t AsDWord();

    int64_t AsSQWord();
    uint64_t AsQWord();

    float AsFloat();
    double AsDouble();

    void Write(Value val);
    Value Read();

    private:
      Value value;
  };

#pragma pack(pop) 

};

#endif // !YL_REGISTER_HPP
