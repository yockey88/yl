require("ymake")

local configuration = {}
configuration.wks_name = "ylang"
configuration.architecture = "x64"
configuration.start_project = "runtime"
configuration.cpp_dialect = "C++latest"
configuration.static_runtime = "On"
configuration.target_dir = "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
configuration.obj_dir = "%{wks.location}/bin_obj/%{cfg.buildcfg}/%{prj.name}"

-- TODO: upgrade platform/configuration system
configuration.build_configurations = { "Debug", "Release" }
configuration.platforms = { "Windows" }

configuration.groups = {
  ["ylang"] = { "./ylang", "./compiler" },
  ["tests"] = { "./tests" }
}

local gtest = {}
gtest.name = "gtest"
gtest.path = "./external/gtest"
gtest.include_dir = "%{wks.location}/external/googletest/include"
gtest.lib_name = "gtest"
gtest.lib_dir = "%{wks.location}/bin/Debug/gtest"
gtest.configurations = "Debug"

local magic_enum = {}
magic_enum.name = "magic_enum"
magic_enum.include_dir = "./external/magic_enum/magic_enum"

local spdlog = {}
spdlog.name = "spdlog"
spdlog.path = "./external/spdlog"
spdlog.include_dir = "./external/spdlog/spdlog"
spdlog.lib_name = "spdlog"
spdlog.lib_dir = "%{wks.location}/bin/Debug/spdlog"

AddDependency(gtest)
AddDependency(magic_enum)
AddDependency(spdlog)
CppWorkspace(configuration)
