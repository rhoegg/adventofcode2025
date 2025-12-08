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
    size_t tachyon_timelines[manifold_width];

    for (size_t i = 0; i < manifold_width; ++i) {
        tachyons[i] = (lines[0][i] == 'S');
        tachyon_timelines[i] = (lines[0][i] == 'S');
    }

    size_t split_count = 0;

    for (size_t i = 1; i < line_count - 1; i++) {
        for (size_t j = 0; j < manifold_width; ++j) {
            if (tachyons[j] && lines[i][j] == '^') {
                tachyons[j] = false;
                size_t timelines = tachyon_timelines[j];
                tachyon_timelines[j] = 0;
                ++split_count;
                if (j > 0) {
                    tachyons[j - 1] = true;
                    tachyon_timelines[j - 1] += timelines;
                }
                if (j < (manifold_width - 1)) {
                    tachyons[j + 1] = true;
                    tachyon_timelines[j + 1] += timelines;
                }
            }
        }
        for (size_t j = 0; j < manifold_width; j++) {
            if (tachyons[j]) printf("%lu", tachyon_timelines[j] % 10);
            else printf("%c", lines[i][j]);
        }
        printf("\n");
    }

    size_t timeline_count = 0;
    for (size_t j = 0; j < manifold_width; j++) {
        timeline_count += tachyon_timelines[j];
    }

    printf("Part 1: %lu splits\n", split_count);
    printf("Part 2: %lu timelines\n", timeline_count);

    free(buf);
}