#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

FILE *open_file(const char *input_path, long *out_len);
char *read_file(const char *input_path, long *out_len);
char **split(char *str, char delimiter, size_t *out_count);
long count_digits(long l);

#endif // COMMON_H