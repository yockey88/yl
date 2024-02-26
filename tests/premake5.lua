local lexer_tests = {}

lexer_tests.name = "lexer_tests"
lexer_tests.path = "./tests"
lexer_tests.kind = "ConsoleApp"
lexer_tests.cppdialect = "C++latest"

lexer_tests.files = function()
  files { "lexer_tests/**.cpp" , "lexer_tests/**.h" }
end

lexer_tests.include_dirs = function()
  includedirs { "tests/lexer_tests" }
end

lexer_tests.components = {}
lexer_tests.components["spdlog"] = "%{wks.location}/external/spdlog"
lexer_tests.components["ylang"] = "%{wks.location}/ylang/src"
lexer_tests.components["gtest"] = "%{wks.location}/external/gtest/googletest/include"

local parser_tests = {}

parser_tests.name = "parser_tests"
parser_tests.path = "./tests"
parser_tests.kind = "ConsoleApp"
parser_tests.cppdialect = "C++latest"

parser_tests.files = function()
  files { "parser_tests/**.cpp" , "parser_tests/**.hpp" }
end

parser_tests.include_dirs = function()
  includedirs { "tests/parser_tests" }
end

parser_tests.components = {}
parser_tests.components["spdlog"] = "%{wks.location}/external/spdlog"
parser_tests.components["ylang"] = "%{wks.location}/ylang/src"
parser_tests.components["gtest"] = "%{wks.location}/external/gtest/googletest/include"

local compiler_tests = {}

compiler_tests.name = "compiler_tests"
compiler_tests.path = "./tests"
compiler_tests.kind = "ConsoleApp"
compiler_tests.cppdialect = "C++latest"

compiler_tests.files = function()
  files { "compiler_tests/**.cpp" , "compiler_tests/**.hpp" }
end

compiler_tests.include_dirs = function()
  includedirs { "tests/compiler_tests" }
end

compiler_tests.components = {}
compiler_tests.components["spdlog"] = "%{wks.location}/external/spdlog"
compiler_tests.components["ylang"] = "%{wks.location}/ylang/src"
compiler_tests.components["gtest"] = "%{wks.location}/external/gtest/googletest/include"

local value_tests = {}
value_tests.name = "value_tests"
value_tests.path = "./tests"
value_tests.kind = "ConsoleApp"
value_tests.cppdialect = "C++latest"

value_tests.files = function()
  files { "value_tests/**.cpp" , "value_tests/**.hpp" }
end

value_tests.include_dirs = function()
  includedirs { "tests/value_tests" }
end

value_tests.components = {}
value_tests.components["spdlog"] = "%{wks.location}/external/spdlog"
value_tests.components["ylang"] = "%{wks.location}/ylang/src"
value_tests.components["gtest"] = "%{wks.location}/external/gtest/googletest/include"

AddProject(compiler_tests)
AddProject(lexer_tests)
AddProject(parser_tests)
AddProject(value_tests)
