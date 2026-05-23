#ifndef UTILS_H
#define UTILS_H

#include <sys/stat.h>

int is_text_file(const char *filename);
long get_file_size(const char *filename);
mode_t get_permissions(const char *filename);

#endif
