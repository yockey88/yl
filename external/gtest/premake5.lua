local gtest = {}

gtest.name = "gtest"
gtest.kind = "StaticLib"
gtest.language = "C++"

gtest.files = function()
  files {
    "googletest/src/**.cc" ,
    "googletest/src/**.h" ,
    "googletest/include/**.h" ,
  }
end

gtest.include_dirs = function()
    includedirs { "./googletest/include" , "./googletest" }
end

AddExternalProject(gtest)
