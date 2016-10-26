function BaseProject(name)
    project(name)
    language "C++"
    kind "ConsoleApp"

    configuration { "Debug" }
        flags
        {
            "Symbols",
        }
end

function CoreProject(name)
    BaseProject(name)
    kind "StaticLib"
end

workspace "PamiEngine"
    configurations { "Debug", "Release" }
    location "_build"

    include "App"
    include "RTTI"
    include 'Utility'
    --include "Clusters"
