#include <vector>
#include <string>
#include <iostream>

#include "RTTI/RTTIBase.h"

#include "luajit-2.0/lua.hpp"

namespace RTTI
{
    class MainCluster : public RTTI::Base
    {
    public:
        MainCluster() : RTTI::Base("MainCluster")
        {

        }

        std::vector<std::string> SubClusters;
    };
}

int main()
{
    std::cout << "Init Lua Vm" << std::endl;

    lua_State* luaState = luaL_newstate();
    if (luaState == NULL)
    {
        std::cerr << "Spark2: lua_newstate() failed to create lua_State." << std::endl;
        return -1;
    }

    luaL_openlibs(luaState);
    system("PAUSE");
    return 0;
}