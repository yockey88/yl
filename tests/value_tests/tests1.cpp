#include "value_tests.hpp"

#include "token.hpp"
#include "value.hpp"
#include "util/io.hpp"

TEST_F(ValueTests, byte_test) {
  ylang::Token token(loc , ylang::TokenType::INTEGER, "123");
  ylang::Value value = ylang::Value::CreateValue(token);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::U8);
  ASSERT_EQ((uint32_t)value.size , ylang::BYTE);

  ASSERT_NO_THROW(value.As<uint8_t>());
  EXPECT_EQ(value.As<uint8_t>(), 123);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<float>(), std::bad_variant_access);
  ASSERT_THROW(value.As<double>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::address_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::RegisterType>(), std::bad_variant_access);
}

TEST_F(ValueTests, word_test) {
  ylang::Token token(loc , ylang::TokenType::INTEGER, "12345");
  ylang::Value value = ylang::Value::CreateValue(token);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::U16);
  ASSERT_EQ((uint32_t)value.size , ylang::WORD);

  ASSERT_NO_THROW(value.As<uint16_t>());
  EXPECT_EQ(value.As<uint16_t>(), 12345);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<float>(), std::bad_variant_access);
  ASSERT_THROW(value.As<double>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::address_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::RegisterType>(), std::bad_variant_access);
}

TEST_F(ValueTests , dword_test) {
  ylang::Token token(loc , ylang::TokenType::INTEGER, "123456789");
  ylang::Value value = ylang::Value::CreateValue(token);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::U32);
  ASSERT_EQ((uint32_t)value.size , ylang::DWORD);

  ASSERT_NO_THROW(value.As<uint32_t>());
  EXPECT_EQ(value.As<uint32_t>(), 123456789);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<float>(), std::bad_variant_access);
  ASSERT_THROW(value.As<double>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::address_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::RegisterType>(), std::bad_variant_access);
}

TEST_F(ValueTests , qword_test) {
  ylang::Token token(loc , ylang::TokenType::INTEGER, "123456789123456789");
  ylang::Value value = ylang::Value::CreateValue(token);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::U64);
  ASSERT_EQ((uint32_t)value.size , ylang::QWORD);

  ASSERT_NO_THROW(value.As<uint64_t>());
  EXPECT_EQ(value.As<uint64_t>(), 123456789123456789);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<float>(), std::bad_variant_access);
  ASSERT_THROW(value.As<double>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::address_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::RegisterType>(), std::bad_variant_access);
}

TEST_F(ValueTests , float_test) {
  ylang::Token token(loc , ylang::TokenType::FLOAT, "123.456");
  ylang::Value value = ylang::Value::CreateValue(token);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::F32);
  ASSERT_EQ((uint32_t)value.size , ylang::DWORD);

  ASSERT_NO_THROW(value.As<float>());
  EXPECT_EQ(value.As<float>(), 123.456f);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<double>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::address_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::RegisterType>(), std::bad_variant_access);
}

TEST_F(ValueTests , double_test) {
  ylang::Token token(loc , ylang::TokenType::DOUBLE, "123456789.123456789");
  ylang::Value value = ylang::Value::CreateValue(token);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)value.size , ylang::QWORD);

  ASSERT_NO_THROW(value.As<double>());
  EXPECT_EQ(value.As<double>(), 123456789.123456789);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<float>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::address_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::RegisterType>(), std::bad_variant_access);
}

TEST_F(ValueTests , address_test) {
  uint64_t address = 0x123456789;
  std::stringstream ss;
  ss << std::dec << address;

  ylang::Token token(loc , ylang::TokenType::ADDRESS, ss.str());
  ylang::Value value = ylang::Value::CreateValue(token);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::ADDRESS);
  EXPECT_EQ((uint32_t)value.size , ylang::QWORD);

  ASSERT_NO_THROW(value.As<ylang::address_t>());
  EXPECT_EQ(value.As<ylang::address_t>().address, 0x123456789);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<float>(), std::bad_variant_access);
  ASSERT_THROW(value.As<double>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::RegisterType>(), std::bad_variant_access);
}

TEST_F(ValueTests , register_test) {
  ylang::RegisterType reg = ylang::RegisterType::RAX;

  ylang::Value value = ylang::Value::CreateValue(reg);

  ylang::printfmt("Value : {}", value);
  ylang::printfmt("Value Data Index : {}" , value.value.index());

  ASSERT_EQ(value.type, ylang::Value::Type::REGISTER);
  EXPECT_EQ((uint32_t)value.size , ylang::QWORD);

  ASSERT_NO_THROW(value.As<ylang::RegisterType>());
  EXPECT_EQ(value.As<ylang::RegisterType>(), ylang::RegisterType::RAX);

  ASSERT_THROW(value.As<int8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint8_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint16_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint32_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<int64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<uint64_t>(), std::bad_variant_access);
  ASSERT_THROW(value.As<float>(), std::bad_variant_access);
  ASSERT_THROW(value.As<double>(), std::bad_variant_access);
  ASSERT_THROW(value.As<ylang::address_t>(), std::bad_variant_access);
}
