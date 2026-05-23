#include <stdio.h>
#include <sys/stat.h>
#include "utils.h"

int is_text_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (!fp)
        return 0;

    int c;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c > 127)
        {
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}

long get_file_size(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (!fp)
        return -1;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    fclose(fp);

    return size;
}

mode_t get_permissions(const char *filename)
{
    struct stat st;

    stat(filename, &st);

    return st.st_mode & 0777;
}
