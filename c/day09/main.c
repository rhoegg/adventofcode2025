#include <stdlib.h>
#include "../common.h"
#include "main.h"

Point tiles[1024];
size_t len_tiles = 0;

void parse_tiles(char *input) {
    char **lines = split(input, '\n', &len_tiles);
    for (size_t i = 0; i < len_tiles; ++i) {
        sscanf(lines[i], "%ld,%ld", &tiles[i].x, &tiles[i].y);
    }
}

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day09/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);
    parse_tiles(buf);

    long largest_area = 0;
    for (size_t i = 0; i < len_tiles; ++i) {
        for (size_t j = 0; j < len_tiles; ++j) {
            long area = labs(tiles[j].x - tiles[i].x + 1) * labs(tiles[j].y - tiles[i].y + 1);
            if (area > largest_area) largest_area = area;
        }
    }
    printf("Part 1: %ld\n", largest_area);

    free(buf);
}