project "sha256"
    kind "StaticLib"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"
    files { "repo/sha256.c" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
