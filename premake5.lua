workspace "dedup"
    configurations { "Debug", "Release" }

include "crypto-algorithms/projects.lua"

project "dedup"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"
    files { "*.c" }
    links { "sha256" }
    includedirs { "crypto-algorithms/repo" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
