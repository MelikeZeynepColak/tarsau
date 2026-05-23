#include <stdio.h>
#include <string.h>
#include "archive.h"
#include "extract.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Kullanim hatasi!\n");
        return 1;
    }

    if (strcmp(argv[1], "-b") == 0)
    {
        char *archive_name = "a.sau";
        char *files[32];
        int file_count = 0;

        for (int i = 2; i < argc; i++)
        {
            if (strcmp(argv[i], "-o") == 0)
            {
                if (i + 1 < argc)
                {
                    archive_name = argv[i + 1];
                    break;
                }
            }
            else
            {
                files[file_count++] = argv[i];
            }
        }

        if (create_archive(archive_name, files, file_count) == 0)
            printf("Dosyalar birlestirildi.\n");
    }
    else if (strcmp(argv[1], "-a") == 0)
    {
        char *archive_name = argv[2];
        char *target_dir = ".";

        if (argc >= 4)
            target_dir = argv[3];

        if (extract_archive(archive_name, target_dir) == 0)
            printf("Dosyalar acildi.\n");
    }
    else
    {
        printf("Gecersiz parametre!\n");
    }

    return 0;
}
