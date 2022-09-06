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

void closerecdir(RECDIR* rd);
RECDIR* openrecdir(str path);
struct dirent* readrecdir(RECDIR* rd);
str recdir_path(RECDIR* rd);
