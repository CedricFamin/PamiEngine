local pathToLuaSrc = os.getcwd() .. "/LuaJit/src";
print("cd " .. pathToLuaSrc .. ";msvcbuild")
--os.execute("cd " .. pathToLuaSrc .. " && %comspec% /k \"C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\Tools\\VsDevCmd.bat\" msvcbuild")
