#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archive.h"
#include "utils.h"

int create_archive(char *archive_name, char *files[], int file_count)
{
    // Maksimum dosya sayısı kontrol edilir
    if (file_count > 32)
    {
        printf("Maksimum 32 dosya olabilir!\n");
        return 1;
    }

    // Toplam dosya boyutunu tutar
    long total_size = 0;

    // Metadata bilgisini tutacak string
    char metadata[10000] = "";

    // Geçici string buffer
    char temp[512];

    // Tüm giriş dosyaları dolaşılır
    for (int i = 0; i < file_count; i++)
    {
        // Dosyanın ASCII text dosyası olup olmadığı kontrol edilir
        if (!is_text_file(files[i]))
        {
            printf("%s giris dosyasinin formati uyumsuzdur!\n", files[i]);

            return 1;
        }

        // Dosya boyutu alınır
        long size = get_file_size(files[i]);

        // Dosya okunamadıysa hata verilir
        if (size < 0)
        {
            printf("%s dosyasi okunamadi!\n", files[i]);

            return 1;
        }

        // Toplam boyut hesaplanır
        total_size += size;

        // Toplam boyut 200MB sınırını geçemez
        if (total_size > 200 * 1024 * 1024)
        {
            printf("Toplam boyut 200MB'i geciyor!\n");

            return 1;
        }

        // Dosyanın izin bilgileri alınır
        mode_t perm = get_permissions(files[i]);

        // Metadata formatı:
        // |dosya_adi,izin,boyut|
        sprintf(temp, "|%s,%o,%ld|", files[i], perm, size);

        // Metadata stringine eklenir
        strcat(metadata, temp);
    }

    // Metadata boyutu hesaplanır
    int metadata_size = strlen(metadata);

    // Arşiv dosyası yazma modunda oluşturulur
    FILE *archive = fopen(archive_name, "w");

    // Dosya oluşturulamadıysa hata verilir
    if (!archive)
    {
        printf("Arsiv dosyasi olusturulamadi!\n");

        return 1;
    }

    // İlk 10 byte metadata boyutunu içerir
    // Örnek:
    // 0000000045
    fprintf(archive, "%010d", metadata_size);

    // Metadata arşive yazılır
    fwrite(metadata, 1, metadata_size, archive);

    // Tüm giriş dosyalarının içerikleri arşive eklenir
    for (int i = 0; i < file_count; i++)
    {
        // Giriş dosyası okuma modunda açılır
        FILE *fp = fopen(files[i], "r");

        // Dosya açılamazsa işlem sonlandırılır
        if (!fp)
        {
            fclose(archive);

            return 1;
        }

        int c;

        // Dosya byte byte okunur
        while ((c = fgetc(fp)) != EOF)
        {
            // Okunan karakter arşive yazılır
            fputc(c, archive);
        }

        // Giriş dosyası kapatılır
        fclose(fp);
    }

    // Arşiv dosyası kapatılır
    fclose(archive);

    return 0;
}
