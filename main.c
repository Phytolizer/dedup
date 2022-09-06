#include "recdir.h"
#include "stb_ds.h"
#include "str.h"

#include <dirent.h>
#include <errno.h>
#include <sha256.h>
#include <sodium/randombytes.h>
#include <stdint.h>
#include <stdio.h>

#define UNUSED(x) (void)(x)

typedef struct
{
    uint8_t value[SHA256_BLOCK_SIZE];
} Hash;

typedef struct
{
    Hash key;
    str* value;
} Record;

static char hex_digit(uint8_t digit)
{
    if (digit < 10)
    {
        return (char)('0' + digit);
    }

    return (char)('a' + (digit - 10));
}

static void hash_as_cstr(Hash hash, char out[static SHA256_BLOCK_SIZE * 2 + 1])
{
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++)
    {
        out[i * 2] = hex_digit(hash.value[i] >> 4);
        out[i * 2 + 1] = hex_digit(hash.value[i] & 0x0F);
    }
    out[(size_t)SHA256_BLOCK_SIZE * 2] = '\0';
}

static void hash_of_file(str file_path, Hash* out)
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
    sha256_final(&ctx, out->value);
}

static void print_files_recursively(str dir_path, Record** db)
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
        Hash hash;
        hash_of_file(path, &hash);
        char cstr[SHA256_BLOCK_SIZE * 2 + 1];
        hash_as_cstr(hash, cstr);
        ptrdiff_t i = stbds_hmgeti(*db, hash);
        if (i < 0)
        {
            Record record = {.key = hash, .value = NULL};
            stbds_arrput(record.value, path);
            stbds_hmputs(*db, record);
        }
        else
        {
            stbds_arrput((*db)[i].value, path);
        }
    }
    recdir_close(recdir);
}

int main(int argc, char** argv)
{
    UNUSED(argc);
    UNUSED(argv);

    size_t seed = 0;
    randombytes_buf(&seed, sizeof(seed));
    stbds_rand_seed(seed);

    Record* db = NULL;

    print_files_recursively(str_lit("."), &db);

    for (ptrdiff_t i = 0; i < stbds_hmlen(db); i++)
    {
        if (stbds_arrlen(db[i].value) > 1)
        {
            char cstr[SHA256_BLOCK_SIZE * 2 + 1];
            hash_as_cstr(db[i].key, cstr);
            printf("%s:\n", cstr);
            for (ptrdiff_t j = 0; j < stbds_arrlen(db[i].value); j++)
            {
                printf("  " STR_FMT "\n", STR_ARG(db[i].value[j]));
            }
        }
    }

    for (ptrdiff_t i = 0; i < stbds_hmlen(db); i++)
    {
        for (ptrdiff_t j = 0; j < stbds_arrlen(db[i].value); j++)
        {
            str_free(db[i].value[j]);
        }
        stbds_arrfree(db[i].value);
    }
    stbds_hmfree(db);
}
