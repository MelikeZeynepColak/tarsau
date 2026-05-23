#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archive.h"
#include "utils.h"

int create_archive(char *archive_name, char *files[], int file_count)
{
    if (file_count > 32)
    {
        printf("Maksimum 32 dosya olabilir!\n");
        return 1;
    }

    long total_size = 0;

    char metadata[10000] = "";
    char temp[512];

    for (int i = 0; i < file_count; i++)
    {
        if (!is_text_file(files[i]))
        {
            printf("%s giris dosyasinin formati uyumsuzdur!\n", files[i]);
            return 1;
        }

        long size = get_file_size(files[i]);

        if (size < 0)
        {
            printf("%s dosyasi okunamadi!\n", files[i]);
            return 1;
        }

        total_size += size;

        if (total_size > 200 * 1024 * 1024)
        {
            printf("Toplam boyut 200MB'i geciyor!\n");
            return 1;
        }

        mode_t perm = get_permissions(files[i]);

        sprintf(temp, "|%s,%o,%ld|", files[i], perm, size);

        strcat(metadata, temp);
    }

    int metadata_size = strlen(metadata);

    FILE *archive = fopen(archive_name, "w");

    if (!archive)
    {
        printf("Arsiv dosyasi olusturulamadi!\n");
        return 1;
    }

    fprintf(archive, "%010d", metadata_size);

    fwrite(metadata, 1, metadata_size, archive);

    for (int i = 0; i < file_count; i++)
    {
        FILE *fp = fopen(files[i], "r");

        if (!fp)
        {
            fclose(archive);
            return 1;
        }

        int c;

        while ((c = fgetc(fp)) != EOF)
        {
            fputc(c, archive);
        }

        fclose(fp);
    }

    fclose(archive);

    return 0;
}
