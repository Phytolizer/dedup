#include "recdir.h"
#include "str.h"

#include <dirent.h>
#include <errno.h>
#include <sha256.h>
#include <stdio.h>

#define UNUSED(x) (void)(x)

static char hex_digit(uint8_t digit)
{
    if (digit < 10)
    {
        return (char)('0' + digit);
    }

    return (char)('a' + (digit - 10));
}

static void hash_as_cstr(BYTE hash[static SHA256_BLOCK_SIZE], char out[static SHA256_BLOCK_SIZE * 2 + 1])
{
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        out[i * 2] = hex_digit(hash[i] >> 4);
        out[i * 2 + 1] = hex_digit(hash[i] & 0x0F);
    }
    out[(size_t)SHA256_BLOCK_SIZE * 2] = '\0';
}

static void hash_of_file(str file_path, BYTE out[static SHA256_BLOCK_SIZE])
{
    FILE* fp = fopen(file_path.ptr, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: could not open " STR_FMT "\n", STR_ARG(file_path));
        exit(1);
    }

    SHA256_CTX ctx = {0};
    sha256_init(&ctx);

    uint8_t buf[1024] = {0};
    while (true)
    {
        size_t nread = fread(buf, 1, sizeof(buf), fp);
        if (ferror(fp))
        {
            fprintf(stderr, "ERROR: could not read " STR_FMT "\n", STR_ARG(file_path));
            exit(1);
        }
        if (nread == 0)
        {
            break;
        }

        sha256_update(&ctx, buf, nread);
    }
    sha256_final(&ctx, out);
}

static void print_files_recursively(str dir_path)
{
    RECDIR* recdir = recdir_open(dir_path);
    if (recdir == NULL)
    {
        return;
    }
    struct dirent* ent;
    while (true)
    {
        errno = 0;
        ent = recdir_read(recdir);
        if (ent == NULL)
        {
            if (errno != 0)
            {
                perror("readrecdir");
            }
            break;
        }
        str path = str_fmt(STR_FMT "/%s", STR_ARG(recdir_path(recdir)), ent->d_name);
        BYTE hash[SHA256_BLOCK_SIZE];
        hash_of_file(path, hash);
        char cstr[SHA256_BLOCK_SIZE * 2 + 1];
        hash_as_cstr(hash, cstr);
        printf(STR_FMT " => %.*s\n", STR_ARG(path), (int)sizeof(cstr) - 1, cstr);
        str_free(path);
    }
    recdir_close(recdir);
}

int main(int argc, char** argv)
{
    UNUSED(argc);
    UNUSED(argv);

    print_files_recursively(str_lit("."));
}
