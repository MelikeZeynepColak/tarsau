#include <stdio.h> 
#include <sys/stat.h> 
#include "utils.h" 

// Dosyanın sadece ASCII (metin) karakterler içerip içermediğini kontrol eder
int is_text_file(const char *filename) 
{ 
    // Dosyayı okuma modunda aç
    FILE *fp = fopen(filename, "r"); 

    // Dosya açılamadıysa metin dosyası değildir
    if (!fp) 
        return 0; 

    int c; 

    // Dosyayı karakter karakter oku
    while ((c = fgetc(fp)) != EOF) 
    { 
        // ASCII dışı (extended char) varsa metin dosyası değildir
        if (c > 127) 
        { 
            fclose(fp); 
            return 0; 
        } 
    } 

    // Dosya başarılı şekilde kontrol edildi
    fclose(fp); 
    return 1; 
} 

// Dosyanın byte cinsinden boyutunu döndürür
long get_file_size(const char *filename) 
{ 
    // Dosyayı aç
    FILE *fp = fopen(filename, "r"); 

    // Dosya açılamadıysa hata döndür
    if (!fp) 
        return -1; 

    // Dosya sonuna git
    fseek(fp, 0, SEEK_END); 

    // Dosya konumunu al (boyut)
    long size = ftell(fp); 

    // Dosyayı kapat
    fclose(fp); 

    // Dosya boyutunu döndür
    return size; 
} 

// Dosyanın izinlerini (chmod permission) döndürür
mode_t get_permissions(const char *filename) 
{ 
    struct stat st; 

    // Dosya bilgilerini al (stat system call)
    stat(filename, &st); 

    // Sadece permission bitlerini (rwx) döndür
    return st.st_mode & 0777; 
}
