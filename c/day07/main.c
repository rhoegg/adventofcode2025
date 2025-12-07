#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include "../common.h"

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day07/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);
    size_t line_count = 0;
    char **lines = split(buf, '\n', &line_count);
    
    const size_t manifold_width = strlen(lines[line_count - 1]);
    bool tachyons[manifold_width];
    for (size_t i = 0; i < manifold_width; ++i) {
        tachyons[i] = (lines[0][i] == 'S');
    }

    size_t split_count = 0;

    for (size_t i = 1; i < line_count - 1; i++) {
        for (size_t j = 0; j < manifold_width; ++j) {
            if (tachyons[j] && lines[i][j] == '^') {
                tachyons[j] = false;
                ++split_count;
                if (j > 0) {
                    tachyons[j - 1] = true;
                }
                if (j < (manifold_width - 1)) {
                    tachyons[j + 1] = true;
                }
            }
        }
        for (size_t j = 0; j < manifold_width; j++) {
            if (tachyons[j]) printf("|");
            else printf("%c", lines[i][j]);
        }
        printf("\n");
    }

    printf("Part 1: %lu splits\n", split_count);

    free(buf);
}