#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

FILE *open_file(const char *input_path, long *out_len) {
   FILE *f = fopen(input_path, "rb");
    if (!f) {
        fprintf(stderr, "Failed to open file: %s\n", input_path);
        exit(EXIT_FAILURE);
    }
    // finding out how big the file is
    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "Failed to seek to end of file: %s\n", input_path);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    long len = ftell(f);
    if (len < 0) {
        fprintf(stderr, "Something wrong with the file, length is negative: %s\n", input_path);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    rewind(f);
    *out_len = len;
    return f;
}

char *read_file(const char *input_path, long *out_len) {
    FILE *f = open_file(input_path, out_len);

    char *buf = malloc(*out_len + 1);
    if (!buf) {
        fprintf(stderr, "Failed to allocate memory for file buffer: %s\n", input_path);
        fclose(f);
        exit(EXIT_FAILURE);
    }

    size_t read_len = fread(buf, 1, *out_len, f);
    if (read_len != (size_t)*out_len) {
        fprintf(stderr, "Failed to read the whole file: %s\n", input_path);
        free(buf);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    buf[*out_len] = '\0';
    fclose(f);
    return buf;
}

char** split(char *str, char delimiter, size_t *out_count) {
    char *start = str;
    char *p = start;
    
    size_t capacity = 8;
    char **tokens = malloc(capacity * sizeof(char *));
    *out_count = 0;
    while (*p) {
        if (delimiter == *p) {
            if (*out_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(tokens, capacity * sizeof(char *));
                if (!temp) return NULL;
                tokens = temp;
            }
            *p = '\0';
            tokens[(*out_count)++] = start;
            start = p + 1;
        }
        ++p;
    }
    if (*out_count >= capacity) {
        capacity += 1;
        char **temp = realloc(tokens, capacity * sizeof(char *));
        if (temp == NULL) return NULL;
        tokens = temp;
    }
    tokens[(*out_count)++] = start;
    return tokens;
}

long count_digits(long l) {
    return log10(l) + 1;
}
