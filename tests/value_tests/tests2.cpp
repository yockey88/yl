#include "value_tests.hpp"

#include "token.hpp"
#include "value.hpp"
#include "util/io.hpp"

TEST_F(ValueTests , arithmetic_test_add_ints) {
  ylang::Token token1(loc , ylang::TokenType::INTEGER, "123");
  ylang::Token token2(loc , ylang::TokenType::INTEGER, "456");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::U16)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::U16]
    << " Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::WORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::WORD]
    << " Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<uint16_t>());
  EXPECT_EQ(result.As<uint16_t>(), 123 + 456);

  ylang::Token token3(loc , ylang::TokenType::INTEGER, "123456789");
  ylang::Token token4(loc , ylang::TokenType::INTEGER, "987654321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::U32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::U32]
    << " Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<uint32_t>());
  EXPECT_EQ(result2.As<uint32_t>(), 123456789 + 987654321);
}

TEST_F(ValueTests , arithmetic_test_add_opp_sign_ints) {
  ylang::Token token1(loc , ylang::TokenType::INTEGER, "123");
  ylang::Token token2(loc , ylang::TokenType::SINTEGER, "-456");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::I16)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::I16]
    << " , Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::WORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::WORD]
    << " , Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<int16_t>());
  EXPECT_EQ(result.As<int16_t>(), 123 - 456);

  ylang::Token token3(loc , ylang::TokenType::SINTEGER, "-123456789");
  ylang::Token token4(loc , ylang::TokenType::INTEGER, "987654321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::U32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::U32]
    << " , Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<uint32_t>());
  EXPECT_EQ(result2.As<uint32_t>(), -123456789 + 987654321);
}

TEST_F(ValueTests , arithmetic_test_add_sints) {
  ylang::Token token1(loc , ylang::TokenType::SINTEGER, "-123");
  ylang::Token token2(loc , ylang::TokenType::SINTEGER, "-456");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::I16)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::I16]
    << " , Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::WORD);

  ASSERT_NO_THROW(result.As<int16_t>());
  EXPECT_EQ((int32_t)result.As<int16_t>(), -123 - 456);

  ylang::Token token3(loc , ylang::TokenType::SINTEGER, "-123456789");
  ylang::Token token4(loc , ylang::TokenType::SINTEGER, "-987654321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::I32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::I32]
    << " , Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::DWORD);

  ASSERT_NO_THROW(result2.As<int32_t>());
  EXPECT_EQ(result2.As<int32_t>(), -123456789 - 987654321);
}

TEST_F(ValueTests , arithmetic_test_add_floats) {
  ylang::Token token1(loc , ylang::TokenType::FLOAT, "123.456");
  ylang::Token token2(loc , ylang::TokenType::FLOAT, "456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F32]
    << " , Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<float>());
  EXPECT_EQ(result.As<float>(), 123.456f + 456.789f);

  ylang::Token token3(loc , ylang::TokenType::FLOAT, "123456.789");
  ylang::Token token4(loc , ylang::TokenType::FLOAT, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F32);
  ASSERT_EQ((uint32_t)result2.size , ylang::DWORD);

  ASSERT_NO_THROW(result2.As<float>());
  EXPECT_LT(result2.As<float>() - (123456.789f + 987654.321f), 0.0001f);
}

TEST_F(ValueTests , arithmetic_test_add_opp_sign_floats) {
  ylang::Token token1(loc , ylang::TokenType::FLOAT, "123.456");
  ylang::Token token2(loc , ylang::TokenType::FLOAT, "-456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F32]
    << " , Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<float>());
  EXPECT_LT(std::abs(result.As<float>() - (123.456f - 456.789f)), 0.0001f);

  ylang::Token token3(loc , ylang::TokenType::FLOAT, "-123456.789");
  ylang::Token token4(loc , ylang::TokenType::FLOAT, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F32]
    << " , Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<float>());
  EXPECT_LT(std::abs(result2.As<float>() - (-123456.789f + 987654.321f)), 0.0001f);
}

TEST_F(ValueTests , arithmetic_test_add_doubles) {
  ylang::Token token1(loc , ylang::TokenType::DOUBLE, "123.456");
  ylang::Token token2(loc , ylang::TokenType::DOUBLE, "456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F64)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F64]
    << " , Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::QWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::QWORD]
    << " , Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<double>());
  EXPECT_EQ(result.As<double>(), 123.456 + 456.789);

  ylang::Token token3(loc , ylang::TokenType::DOUBLE, "123456.789");
  ylang::Token token4(loc , ylang::TokenType::DOUBLE, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)result2.size , ylang::QWORD);

  ASSERT_NO_THROW(result2.As<double>());
  EXPECT_EQ(result2.As<double>(), 123456.789 + 987654.321);
}

TEST_F(ValueTests , arithmetic_test_add_opp_sign_doubles) {
  ylang::Token token1(loc , ylang::TokenType::DOUBLE, "123.456");
  ylang::Token token2(loc , ylang::TokenType::DOUBLE, "-456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)result.size , ylang::QWORD);

  ASSERT_NO_THROW(result.As<double>());
  ASSERT_LT(std::abs(result.As<double>() - (123.456 - 456.789)), 0.0001);

  ylang::Token token3(loc , ylang::TokenType::DOUBLE, "-123456.789");
  ylang::Token token4(loc , ylang::TokenType::DOUBLE, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)result2.size , ylang::QWORD);

  ASSERT_NO_THROW(result2.As<double>());
  ASSERT_LT(std::abs(result2.As<double>() - (-123456.789 + 987654.321)), 0.0001);
}

TEST_F(ValueTests , arithmetic_add_int_float) {
  ylang::Token token1(loc , ylang::TokenType::INTEGER, "123");
  ylang::Token token2(loc , ylang::TokenType::FLOAT, "456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F32]
    << " , Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<float>());
  EXPECT_EQ(result.As<float>(), 123 + 456.789f);

  ylang::Token token3(loc , ylang::TokenType::SINTEGER, "-123456789");
  ylang::Token token4(loc , ylang::TokenType::FLOAT, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F32]
    << " , Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<float>());
  EXPECT_LT(result2.As<float>() - (-123456789.f + 987654.321f), 0.0001f);
}

TEST_F(ValueTests , arithmetic_add_sint_float) {
  ylang::Token token1(loc , ylang::TokenType::SINTEGER, "-123");
  ylang::Token token2(loc , ylang::TokenType::FLOAT, "456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F32]
    << " , Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<float>());
  EXPECT_EQ(result.As<float>(), -123 + 456.789f);

  ylang::Token token3(loc , ylang::TokenType::SINTEGER, "-123456789");
  ylang::Token token4(loc , ylang::TokenType::FLOAT, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::F32]
    << " , Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " , Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<float>());
  EXPECT_LT(result2.As<float>() - (-123456789.f + 987654.321f), 0.0001f);
}

TEST_F(ValueTests , arithmetic_add_int_double) {
  ylang::Token token1(loc , ylang::TokenType::INTEGER, "123");
  ylang::Token token2(loc , ylang::TokenType::DOUBLE, "456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)result.size , ylang::QWORD);

  ASSERT_NO_THROW(result.As<double>());
  EXPECT_EQ(result.As<double>(), 123 + 456.789);

  ylang::Token token3(loc , ylang::TokenType::SINTEGER, "-123456789");
  ylang::Token token4(loc , ylang::TokenType::DOUBLE, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)result2.size , ylang::QWORD);

  ASSERT_NO_THROW(result2.As<double>());
  EXPECT_LT(result2.As<double>() - (-123456789 + 987654.321), 0.0001);
}

TEST_F(ValueTests , arithmetic_add_sint_double) {
  ylang::Token token1(loc , ylang::TokenType::SINTEGER, "-123");
  ylang::Token token2(loc , ylang::TokenType::DOUBLE, "456.789");
  ylang::Value value1 = ylang::Value::CreateValue(token1);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 + value2);

  ylang::printfmt("Value1 : {}", value1);
  ylang::printfmt("Value2 : {}", value2);
  ylang::printfmt("Result : {}", result);

  ASSERT_EQ(result.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)result.size , ylang::QWORD);

  ASSERT_NO_THROW(result.As<double>());
  EXPECT_EQ(result.As<double>(), -123 + 456.789);

  ylang::Token token3(loc , ylang::TokenType::SINTEGER, "-123456789");
  ylang::Token token4(loc , ylang::TokenType::DOUBLE, "987654.321");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 + value4);

  ylang::printfmt("Value3 : {}", value3);
  ylang::printfmt("Value4 : {}", value4);
  ylang::printfmt("Result2 : {}", result2);

  ASSERT_EQ(result2.type, ylang::Value::Type::F64);
  ASSERT_EQ((uint32_t)result2.size , ylang::QWORD);

  ASSERT_NO_THROW(result2.As<double>());
  EXPECT_LT(result2.As<double>() - (-123456789 + 987654.321), 0.0001);
}
