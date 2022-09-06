#pragma once

#include "str.h"

#include <dirent.h>

typedef struct
{
    DIR* dir;
    str path;
} RECDIR_Frame;

typedef BUF(RECDIR_Frame) Dirs;

typedef struct
{
    Dirs dirs;
} RECDIR;

void recdir_close(RECDIR* rd);
RECDIR* recdir_open(str path);
struct dirent* recdir_read(RECDIR* rd);
str recdir_path(RECDIR* rd);
