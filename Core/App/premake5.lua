BaseProject "App"

  files { "**.cpp", "**.h" }

  links { "lua51", "RTTI" }

  includedirs
  {
      ".",
      "..",
      "../../Externals/LuaJit/LuaJIT/_build/include"
  }

  libdirs
  {
      "../../Externals/LuaJit/LuaJIT/_build/lib/lua/5.1"
  }
