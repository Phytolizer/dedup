#include "recdir.h"

#include <errno.h>

static int recdir_push(RECDIR* rd, str dir_path)
{
    BUF_PUSH(&rd->dirs, ((RECDIR_Frame){.dir = opendir(str_ptr(dir_path)), .path = dir_path}));
    if (recdir_top(rd)->dir == NULL)
    {
        str_free(dir_path);
        (void)BUF_POP(&rd->dirs);
        return -1;
    }
    return 0;
}

static void recdir_pop(RECDIR* rd)
{
    closedir(recdir_top(rd)->dir);
    str_free(recdir_top(rd)->path);
    (void)BUF_POP(&rd->dirs);
}

RECDIR_Frame* recdir_top(RECDIR* rd)
{
    return &(rd)->dirs.ptr[(rd)->dirs.len - 1];
}

void recdir_close(RECDIR* rd)
{
    for (size_t i = 0; i < rd->dirs.len; ++i)
    {
        closedir(rd->dirs.ptr[i].dir);
        str_free(rd->dirs.ptr[i].path);
    }
    BUF_FREE(rd->dirs);
    free(rd);
}

RECDIR* recdir_open(str path)
{
    RECDIR* result = malloc(sizeof(RECDIR));
    result->dirs = (Dirs)BUF_NEW;
    if (recdir_push(result, path) < 0)
    {
        recdir_close(result);
        return NULL;
    }
    return result;
}

struct dirent* recdir_read(RECDIR* rd)
{
    while (rd->dirs.len > 0)
    {
        RECDIR_Frame* top = recdir_top(rd);
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
    return str_ref(recdir_top(rd)->path);
}
