target "sha256"
    set_kind "static"
    add_files "repo/sha256.c"
    set_targetdir "$(buildir)/lib"
