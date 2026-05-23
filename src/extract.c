#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "extract.h"

// Arşiv içerisindeki dosya bilgilerini tutan yapı
typedef struct
{
    // Dosya adı
    char filename[256];

    // Dosya izinleri
    int permissions;

    // Dosya boyutu
    long size;

} FileInfo;

int extract_archive(char *archive_name, char *target_dir)
{
    // Dosya uzantısını almak için son '.' karakteri bulunur
    char *ext = strrchr(archive_name, '.');

    // Eğer uzantı yoksa veya .sau değilse hata verilir
    if (!ext || strcmp(ext, ".sau") != 0)
    {
        printf("Arsiv dosyasi uygunsuz veya bozuk!\n");
        return 1;
    }

    // Arşiv dosyası okuma modunda açılır
    FILE *archive = fopen(archive_name, "r");

    // Dosya açılamadıysa hata verilir
    if (!archive)
    {
        printf("Arsiv dosyasi uygunsuz veya bozuk!\n");
        return 1;
    }

    // Hedef dizin oluşturulur
    // Eğer zaten varsa sorun oluşturmaz
    mkdir(target_dir, 0777);

    // İlk 10 byte metadata boyutunu tutar
    char size_buffer[11];

    // İlk 10 byte okunur
    fread(size_buffer, 1, 10, archive);

    // String sonlandırıcı eklenir
    size_buffer[10] = '\0';

    // Metadata boyutu integer değere çevrilir
    int metadata_size = atoi(size_buffer);

    // Geçersiz metadata boyutu kontrolü
    if (metadata_size <= 0)
    {
        fclose(archive);

        printf("Arsiv dosyasi uygunsuz veya bozuk!\n");

        return 1;
    }

    // Metadata için dinamik bellek ayrılır
    char *metadata = malloc(metadata_size + 1);

    // Metadata okunur
    fread(metadata, 1, metadata_size, archive);

    // String sonlandırıcı eklenir
    metadata[metadata_size] = '\0';

    // Maksimum 32 dosya bilgisi tutulur
    FileInfo files[32];

    // Dosya sayacı
    int file_count = 0;

    // Metadata içerisindeki kayıtlar | karakterine göre ayrılır
    char *token = strtok(metadata, "|");

    // Tüm kayıtlar işlenir
    while (token != NULL)
    {
        // Her kayıttan:
        // dosya adı, izinler ve boyut okunur
        sscanf(token, "%255[^,],%o,%ld",
               files[file_count].filename,
               &files[file_count].permissions,
               &files[file_count].size);

        // Dosya sayısı artırılır
        file_count++;

        // Sonraki kayıt alınır
        token = strtok(NULL, "|");
    }

    // Tüm dosyalar arşivden çıkarılır
    for (int i = 0; i < file_count; i++)
    {
        char path[512];

        // Hedef dosya yolu oluşturulur
        sprintf(path, "%s/%s", target_dir, files[i].filename);

        // Çıkış dosyası oluşturulur
        FILE *out = fopen(path, "w");

        // Dosya oluşturulamazsa sonraki dosyaya geçilir
        if (!out)
            continue;

        // Dosya içeriği byte byte okunup yazılır
        for (long j = 0; j < files[i].size; j++)
        {
            int c = fgetc(archive);

            // Beklenmedik dosya sonu kontrolü
            if (c == EOF)
                break;

            // Karakter hedef dosyaya yazılır
            fputc(c, out);
        }

        // Çıkış dosyası kapatılır
        fclose(out);

        // Orijinal dosya izinleri geri yüklenir
        chmod(path, files[i].permissions);
    }

    // Metadata belleği serbest bırakılır
    free(metadata);

    // Arşiv dosyası kapatılır
    fclose(archive);

    return 0;
}
