local compiler = {}

compiler.name = "compiler"
compiler.path = "./commpiler"
compiler.kind = "ConsoleApp"
compiler.cppdialect = "C++latest"

compiler.files = function()
  files { "src/*.cpp" , "src/*.h" }
end

compiler.include_dirs = function()
  includedirs { "src" }
end

compiler.components = {}
compiler.components["spdlog"] = { "%{wks.location}/external/spdlog" }
compiler.components["ylang"] = { "%{wks.location}/ylang/src" }

AddProject(compiler)
