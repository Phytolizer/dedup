#include "recdir.h"
#include "str.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>

#define UNUSED(x) (void)(x)

static void print_files_recursively(str dir_path)
{
    RECDIR* recdir = openrecdir(dir_path);
    if (recdir == NULL)
    {
        return;
    }
    struct dirent* ent;
    while (true)
    {
        errno = 0;
        ent = readrecdir(recdir);
        if (ent == NULL)
        {
            if (errno != 0)
            {
                perror("readrecdir");
            }
            break;
        }
        printf(STR_FMT "/%s\n", STR_ARG(recdir_path(recdir)), ent->d_name);
    }
    closerecdir(recdir);
}

int main(int argc, char** argv)
{
    UNUSED(argc);
    UNUSED(argv);

    print_files_recursively(str_lit("."));
}
