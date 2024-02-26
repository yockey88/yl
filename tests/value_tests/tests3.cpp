#include "value_tests.hpp"

#include "token.hpp"
#include "value.hpp"
#include "util/io.hpp"

TEST_F(ValueTests , arithmetic_test_sub_ints) {
  ylang::Token token(loc , ylang::TokenType::INTEGER , "5");
  ylang::Token token2(loc , ylang::TokenType::INTEGER , "3");
  ylang::Value value1 = ylang::Value::CreateValue(token);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result = value1 - value2;
  ASSERT_NO_THROW(result = value1 - value2);

  ylang::printfmt("Value1 : {}" , value1);
  ylang::printfmt("Value2 : {}" , value2);
  ylang::printfmt("Result : {}" , result);

  ASSERT_EQ(result.type , ylang::Value::Type::U8)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::U8]
    << "Actual : " << ylang::kValueStrings[result.type];
  EXPECT_EQ((uint32_t)result.size , ylang::BYTE)
    << "Expected : " << ylang::WordSizeStrings[ylang::BYTE]
    << "Actual : " << ylang::WordSizeStrings[result.size];
  
  ASSERT_NO_THROW(result.As<uint8_t>());
  ASSERT_EQ(result.As<uint8_t>() , 2);

  ylang::Token token3(loc , ylang::TokenType::INTEGER , "10000");
  ylang::Token token4(loc , ylang::TokenType::INTEGER , "5000");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 - value4);

  ylang::printfmt("Value3 : {}" , value3);
  ylang::printfmt("Value4 : {}" , value4);
  ylang::printfmt("Result2 : {}" , result2);

  ASSERT_EQ(result2.type , ylang::Value::Type::U16)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::U16]
    << " Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::WORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::WORD]
    << " Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<uint16_t>());
  EXPECT_EQ(result2.As<uint16_t>() , 5000);
} 

TEST_F(ValueTests , arithmetic_test_sub_ints2) {
  ylang::Token token(loc , ylang::TokenType::INTEGER , "5");
  ylang::Token token2(loc , ylang::TokenType::INTEGER , "8");
  ylang::Value value1 = ylang::Value::CreateValue(token);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 - value2);

  ylang::printfmt("Value1 : {}" , value1);
  ylang::printfmt("Value2 : {}" , value2);
  ylang::printfmt("Result : {}" , result);

  ASSERT_EQ(result.type , ylang::Value::Type::I8)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::I8]
    << " Actual : " << ylang::kValueStrings[result.type];
  EXPECT_EQ((uint32_t)result.size , ylang::BYTE)
    << "Expected : " << ylang::WordSizeStrings[ylang::BYTE]
    << " Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<int8_t>());
  ASSERT_EQ(result.As<int8_t>() , -3);

  ylang::Token token3(loc , ylang::TokenType::INTEGER , "10000000");
  ylang::Token token4(loc , ylang::TokenType::INTEGER , "15000000");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 - value4);

  ylang::printfmt("Value3 : {}" , value3);
  ylang::printfmt("Value4 : {}" , value4);
  ylang::printfmt("Result2 : {}" , result2);

  ASSERT_EQ(result2.type , ylang::Value::Type::I32)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::I32]
    << " Actual : " << ylang::kValueStrings[result2.type];
  EXPECT_EQ((uint32_t)result2.size , ylang::DWORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::DWORD]
    << " Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<int32_t>());
  EXPECT_EQ(result2.As<int32_t>() , -5000000);

  ylang::Token token5(loc , ylang::TokenType::INTEGER , "2");
  ylang::Token token6(loc , ylang::TokenType::INTEGER , "3");
  ylang::Value value5 = ylang::Value::CreateValue(token5);
  ylang::Value value6 = ylang::Value::CreateValue(token6);

  ylang::Value result3;
  ASSERT_NO_THROW(result3 = value5 - value6);

  ylang::printfmt("Value5 : {}" , value5);
  ylang::printfmt("Value6 : {}" , value6);
  ylang::printfmt("Result3 : {}" , result3);

  ASSERT_EQ(result3.type , ylang::Value::Type::I8)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::I8]
    << " Actual : " << ylang::kValueStrings[result3.type];
  EXPECT_EQ((uint32_t)result3.size , ylang::BYTE)
    << "Expected : " << ylang::WordSizeStrings[ylang::BYTE]
    << " Actual : " << ylang::WordSizeStrings[result3.size];

  ASSERT_NO_THROW(result3.As<int8_t>());
  EXPECT_EQ(result3.As<int8_t>() , -1);
}

TEST_F(ValueTests , arithmetic_test_sub_opp_sign_ints) {
  ylang::Token token(loc , ylang::TokenType::INTEGER , "5");
  ylang::Token token2(loc , ylang::TokenType::SINTEGER , "-3");
  ylang::Value value1 = ylang::Value::CreateValue(token);
  ylang::Value value2 = ylang::Value::CreateValue(token2);

  ylang::Value result;
  ASSERT_NO_THROW(result = value1 - value2);

  ylang::printfmt("Value1 : {}" , value1);
  ylang::printfmt("Value2 : {}" , value2);
  ylang::printfmt("Result : {}" , result);

  ASSERT_EQ(result.type , ylang::Value::Type::U8)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::U8]
    << " Actual : " << ylang::kValueStrings[result.type];
  ASSERT_EQ((uint32_t)result.size , ylang::BYTE)
    << "Expected : " << ylang::WordSizeStrings[ylang::BYTE]
    << " Actual : " << ylang::WordSizeStrings[result.size];

  ASSERT_NO_THROW(result.As<uint8_t>());
  ASSERT_EQ(result.As<uint8_t>() , 8);

  ylang::Token token3(loc , ylang::TokenType::SINTEGER , "-10000");
  ylang::Token token4(loc , ylang::TokenType::INTEGER , "5000");
  ylang::Value value3 = ylang::Value::CreateValue(token3);
  ylang::Value value4 = ylang::Value::CreateValue(token4);

  ylang::Value result2;
  ASSERT_NO_THROW(result2 = value3 - value4);

  ylang::printfmt("Value3 : {}" , value3);
  ylang::printfmt("Value4 : {}" , value4);
  ylang::printfmt("Result2 : {}" , result2);

  ASSERT_EQ(result2.type , ylang::Value::Type::I16)
    << "Expected : " << ylang::kValueStrings[ylang::Value::Type::I16]
    << " Actual : " << ylang::kValueStrings[result2.type];
  ASSERT_EQ((uint32_t)result2.size , ylang::WORD)
    << "Expected : " << ylang::WordSizeStrings[ylang::WORD]
    << " Actual : " << ylang::WordSizeStrings[result2.size];

  ASSERT_NO_THROW(result2.As<int16_t>());
  EXPECT_EQ(result2.As<int16_t>() , -15000);
}
