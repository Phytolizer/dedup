set_defaultmode "dbg"
includes "crypto-algorithms"

target "dedup"
    set_kind "binary"
    add_files "*.c"
    add_cflags {
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wmissing-prototypes",
        "-Wno-unused-function",
        "-std=c11",
        "-D_DEFAULT_SOURCE",
        "-ggdb",
        "-fsanitize=address",
        "-fno-omit-frame-pointer",
    }
    add_ldflags { "-fsanitize=address" }
    add_links { "sha256" }
    add_linkdirs "$(buildir)/lib"
    add_includedirs "crypto-algorithms/repo"
