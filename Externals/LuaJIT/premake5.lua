local pathToLuaSrc = os.getcwd() .. "/LuaJit-2.0.4";
print("cd " .. pathToLuaSrc .. ";msvcbuild")
os.execute("cd " .. pathToLuaSrc .. " && make")
