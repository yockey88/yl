#include "env_tests.hpp"

#include "value.hpp"
#include "env/environment.hpp"
#include "interpreter/interpreter.hpp"

TEST_F(EnvironmentTests , clone_tests1) {
  std::unique_ptr<ylang::Environment> env1 = std::make_unique<ylang::Environment>(); 

  ylang::Token one{ylang::SourceLocation{} , ylang::TokenType::INTEGER , "1"};
  ylang::Token two{ylang::SourceLocation{} , ylang::TokenType::INTEGER , "2"};
  ylang::Token three{ylang::SourceLocation{} , ylang::TokenType::INTEGER , "3"};

  env1->Define("a" , ylang::Value::CreateValue(one));
  env1->Define("b" , ylang::Value::CreateValue(two));

  auto env2 = env1->CreateClosure();
  
  env2->Define("c" , ylang::Value::CreateValue(three));

  auto& vara = env2->Get("a");
  auto& varb = env2->Get("b");
  auto& varc = env2->Get("c");

  ASSERT_TRUE(vara.val.IsInt());
  ASSERT_TRUE(varb.val.IsInt());
  ASSERT_TRUE(varc.val.IsInt());

  ASSERT_EQ(vara.val.AsUInt() , 1);
  ASSERT_EQ(varb.val.AsUInt() , 2);
  ASSERT_EQ(varc.val.AsUInt() , 3);

  auto env3 = env2->CreateClosure();

  auto& vara2 = env3->Get("a");
  auto& varb2 = env3->Get("b");
  auto& varc2 = env3->Get("c");

  ASSERT_TRUE(vara2.val.IsInt());
  ASSERT_TRUE(varb2.val.IsInt());
  ASSERT_TRUE(varc2.val.IsInt());

  ASSERT_EQ(vara2.val.AsUInt() , 1);
  ASSERT_EQ(varb2.val.AsUInt() , 2);
  ASSERT_EQ(varc2.val.AsUInt() , 3);

  auto env4 = env3->CreateClosure();

  auto& vara3 = env4->Get("a");
  auto& varb3 = env4->Get("b");
  auto& varc3 = env4->Get("c");

  ASSERT_TRUE(vara3.val.IsInt());
  ASSERT_TRUE(varb3.val.IsInt());
  ASSERT_TRUE(varc3.val.IsInt());

  ASSERT_EQ(vara3.val.AsUInt() , 1);
  ASSERT_EQ(varb3.val.AsUInt() , 2);
  ASSERT_EQ(varc3.val.AsUInt() , 3);
}

TEST_F(EnvironmentTests , clone_tests2_closures) {
  std::unique_ptr<ylang::Environment> env1 = std::make_unique<ylang::Environment>();
  // fmt::print(fmt::runtime("env1 address : {:p}"), env1.get());

  ylang::BlockStmt* body = new ylang::BlockStmt{{}};

  ylang::Token func_name{ylang::SourceLocation{} , ylang::TokenType::IDENTIFIER , "func"};
  ylang::Token type{ylang::SourceLocation{} , ylang::TokenType::IDENTIFIER , "void"};

  ylang::FunctionStmt* stmt = new ylang::FunctionStmt{ func_name , {} , type , body };

  {
    auto new_env = env1->CreateClosure();
    std::unique_ptr<ylang::Callable> callable = std::make_unique<ylang::Function>(*stmt , new_env);

    [[maybe_unused]] auto cloned = callable->Clone();
  }

  delete stmt;
}

int main(int argc , char* argv[]) {
  ::testing::InitGoogleTest(&argc , argv);
  return RUN_ALL_TESTS();
}
