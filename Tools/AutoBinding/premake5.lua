project "AutoBinding"

  install(function(prj)
      os.copyfile("Tools/_build/bin/x64/Release/AutoBinding.exe", "Utils/AutoBind.exe")
      os.copydir("Tools/AutoBinding/Templates", "Utils/Templates")
  end)

  kind "ConsoleApp"
  language "C++"

  platforms { "x64" }

  files { "**.cpp", "**.h" }

  excludes { "Templates/*"}

  includedirs
  {
      ".",
      "../../Externals/llvm/tools/clang/include",
      "../../Externals/llvm/include",
      "../../Externals/",
  }

  links
  {
    "LLVMSupport",
    "gtest_main",
    "gtest",
    "clangAST",
    "clangASTMatchers",
    "clangBasic",
    "clangFrontend",
    "clangTooling",
    "clangParse",
    "LLVMMCParser",
    "clangSerialization",
    "clangSema",
    "clangEdit",
    "clangAnalysis",
    "LLVMBitReader",
    "LLVMProfileData",
    "clangDriver",
    "LLVMOption",
    "clangFormat",
    "clangToolingCore",
    "clangRewrite",
    "clangLex",
    "LLVMCore",
    "LLVMMC",
    "version",
  }

  configuration { "Release" }
      libdirs
      {
          "../../Externals/llvm/Release/lib",
      }

  configuration { "Debug" }
      libdirs
      {
          "../../Externals/llvm/Debug/lib",
      }

      defines { "_SCL_SECURE_NO_WARNINGS" }

      flags
      {
          "Symbols",
      }
