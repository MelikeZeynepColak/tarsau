#include <stdio.h>
#include <string.h>
#include "archive.h"
#include "extract.h"

int main(int argc, char *argv[])
{
    // Program en az 3 argüman almalıdır
    // Örnek:
    // ./tarsau -b file1.txt
    // ./tarsau -a archive.sau
    if (argc < 3)
    {
        printf("Kullanim hatasi!\n");
        return 1;
    }

    // Eğer kullanıcı -b parametresi verdiyse
    // arşiv oluşturma işlemi yapılır
    if (strcmp(argv[1], "-b") == 0)
    {
        // Varsayılan arşiv adı
        char *archive_name = "a.sau";

        // Maksimum 32 giriş dosyası tutulacak
        char *files[32];

        // Dosya sayısını tutar
        int file_count = 0;

        // Komut satırındaki parametreler dolaşılır
        for (int i = 2; i < argc; i++)
        {
            // Eğer -o parametresi verilmişse
            // sonraki argüman arşiv adı olur
            if (strcmp(argv[i], "-o") == 0)
            {
                // Arşiv adı gerçekten girilmiş mi kontrol edilir
                if (i + 1 < argc)
                {
                    archive_name = argv[i + 1];
                    break;
                }
            }
            else
            {
                // Normal dosya isimleri files dizisine eklenir
                files[file_count++] = argv[i];
            }
        }

        // Arşiv oluşturma fonksiyonu çağrılır
        // Başarılı olursa bilgi mesajı yazdırılır
        if (create_archive(archive_name, files, file_count) == 0)
            printf("Dosyalar birlestirildi.\n");
    }

    // Eğer kullanıcı -a parametresi verdiyse
    // arşiv açma işlemi yapılır
    else if (strcmp(argv[1], "-a") == 0)
    {
        // Açılacak arşiv dosyası adı
        char *archive_name = argv[2];

        // Varsayılan hedef dizin mevcut dizindir
        char *target_dir = ".";

        // Eğer kullanıcı hedef dizin girdiyse kullanılır
        if (argc >= 4)
            target_dir = argv[3];

        // Arşiv açma fonksiyonu çağrılır
        // Başarılı olursa bilgi mesajı yazdırılır
        if (extract_archive(archive_name, target_dir) == 0)
            printf("Dosyalar acildi.\n");
    }

    // Geçersiz parametre girilmişse hata mesajı verilir
    else
    {
        printf("Gecersiz parametre!\n");
    }

    return 0;
}
