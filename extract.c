#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "extract.h"

typedef struct
{
    char filename[256];
    int permissions;
    long size;

} FileInfo;

int extract_archive(char *archive_name, char *target_dir)
{
    char *ext = strrchr(archive_name, '.');

    if (!ext || strcmp(ext, ".sau") != 0)
    {
        printf("Arsiv dosyasi uygunsuz veya bozuk!\n");
        return 1;
    }

    FILE *archive = fopen(archive_name, "r");

    if (!archive)
    {
        printf("Arsiv dosyasi uygunsuz veya bozuk!\n");
        return 1;
    }

    mkdir(target_dir, 0777);

    char size_buffer[11];

    fread(size_buffer, 1, 10, archive);

    size_buffer[10] = '\0';

    int metadata_size = atoi(size_buffer);

    if (metadata_size <= 0)
    {
        fclose(archive);
        printf("Arsiv dosyasi uygunsuz veya bozuk!\n");
        return 1;
    }

    char *metadata = malloc(metadata_size + 1);

    fread(metadata, 1, metadata_size, archive);

    metadata[metadata_size] = '\0';

    FileInfo files[32];

    int file_count = 0;

    char *token = strtok(metadata, "|");

    while (token != NULL)
    {
        sscanf(token, "%255[^,],%o,%ld",
               files[file_count].filename,
               &files[file_count].permissions,
               &files[file_count].size);

        file_count++;

        token = strtok(NULL, "|");
    }

    for (int i = 0; i < file_count; i++)
    {
        char path[512];

        sprintf(path, "%s/%s", target_dir, files[i].filename);

        FILE *out = fopen(path, "w");

        if (!out)
            continue;

        for (long j = 0; j < files[i].size; j++)
        {
            int c = fgetc(archive);

            if (c == EOF)
                break;

            fputc(c, out);
        }

        fclose(out);

        chmod(path, files[i].permissions);
    }

    free(metadata);

    fclose(archive);

    return 0;
}
