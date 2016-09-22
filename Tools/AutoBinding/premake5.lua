project "AutoBinding"
  kind "ConsoleApp"
  language "C++"

  platforms { "x64" }

  files { "**.cpp", "**.h" }

  includedirs
  {
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
          "../../Externals/Jinja2CPPLight/_build/bin/x64/Release",
      }

  configuration { "Debug" }
      libdirs
      {
          "../../Externals/llvm/Debug/lib",
          "../../Externals/Jinja2CPPLight/_build/bin/x64/Release",
      }

      defines { "_SCL_SECURE_NO_WARNINGS" }

      flags
      {
          "Symbols",
      }
