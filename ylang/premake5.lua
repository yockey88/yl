local ylang = {}

ylang.name = "ylang"
ylang.path = "./ylang"
ylang.kind = "StaticLib"
ylang.language = "C++"
ylang.cppdialect = "C++latest"
ylang.staticruntime = "On"

ylang.files = function()
  files {
    "src/**.cpp",
    "src/**.h",
  }
end

ylang.include_dirs = function()
  includedirs {
    "./src"
  }
end

ylang.components = {}
ylang.components["spdlog"] = "%{wks.location}/external/spdlog/"

AddProject(ylang)
