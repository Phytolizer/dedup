#include "recdir.h"

#include <errno.h>

#define TOP(rd) (&(rd)->dirs.ptr[(rd)->dirs.len - 1])

static int recdir_push(RECDIR* rd, str dir_path)
{
    BUF_PUSH(&rd->dirs, ((RECDIR_Frame){.dir = opendir(str_ptr(dir_path)), .path = dir_path}));
    if (TOP(rd)->dir == NULL)
    {
        str_free(dir_path);
        (void)BUF_POP(&rd->dirs);
        return -1;
    }
    return 0;
}

static void recdir_pop(RECDIR* rd)
{
    closedir(TOP(rd)->dir);
    str_free(TOP(rd)->path);
    (void)BUF_POP(&rd->dirs);
}

void closerecdir(RECDIR* rd)
{
    for (size_t i = 0; i < rd->dirs.len; ++i)
    {
        closedir(rd->dirs.ptr[i].dir);
        str_free(rd->dirs.ptr[i].path);
    }
    BUF_FREE(rd->dirs);
    free(rd);
}

RECDIR* openrecdir(str path)
{
    RECDIR* result = malloc(sizeof(RECDIR));
    result->dirs = (Dirs)BUF_NEW;
    if (recdir_push(result, path) < 0)
    {
        closerecdir(result);
        return NULL;
    }
    return result;
}

struct dirent* readrecdir(RECDIR* rd)
{
    while (rd->dirs.len > 0)
    {
        RECDIR_Frame* top = TOP(rd);
        struct dirent* ent = readdir(top->dir);
        if (ent != NULL)
        {
            if (ent->d_type == DT_DIR)
            {
                if (str_eq(str_ref(ent->d_name), str_lit(".")) || str_eq(str_ref(ent->d_name), str_lit("..")))
                {
                    continue;
                }
                recdir_push(rd, path_join(str_ref(top->path), str_ref(ent->d_name)));
                continue;
            }
            return ent;
        }
        if (errno != 0)
        {
            return NULL;
        }
        recdir_pop(rd);
    }
    return NULL;
}

str recdir_path(RECDIR* rd)
{
    return str_ref(TOP(rd)->path);
}
