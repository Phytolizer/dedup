set_defaultmode "dbg"
includes "crypto-algorithms"
add_requires "libsodium"

target "dedup"
    set_kind "binary"
    add_files "*.c"
    add_cflags {
        "-Wall",
        "-Wextra",
        "-Wpedantic",
        "-Wmissing-prototypes",
        "-Wno-unused-function",
        "-std=gnu11",
        "-ggdb",
        "-fsanitize=address",
        "-fno-omit-frame-pointer",
    }
    add_defines "STBDS_NO_SHORT_NAMES"
    add_ldflags { "-fsanitize=address" }
    add_links { "sha256" }
    add_packages { "libsodium" }
    add_linkdirs "$(buildir)/lib"
    add_includedirs "crypto-algorithms/repo"
