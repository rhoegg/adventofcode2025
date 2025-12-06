#include <stdio.h>
#include <stdlib.h>
#include "../common.h"

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day01/sample.txt";
    long len;
    char *buf = read_file(input_path, &len);

    // parse the file
    size_t capacity = 64;
    size_t count = 0;
    int* rotations = malloc(capacity * sizeof(int));
    char *end = buf + len;

    for (char *p = buf; p < end; ++p) {
        if (*p == 'L' || *p == 'R') {
            if (count >= capacity) {
                capacity *= 2;
                rotations = realloc(rotations, capacity * sizeof(int));
            }
            int sign = (*p == 'L') ? -1 : 1;
            ++p;
            int value = 0;
            while (p < end && *p >= '0' && *p <= '9') {
                value = value * 10 + (*p - '0');
                ++p;
            }
            --p; // adjust for the extra increment in the inner loop
            rotations[count++] = sign * value;
        }
    }

    // compute the password
    int dial = 50;
    int zeros = 0;
    int next = 50;
    int total_clicks = 0;
    for (size_t i = 0; i < count; ++i) {
        next = (dial + rotations[i] + 1000) % 100;
        int clicks = abs(rotations[i]) / 100;
        if (next == 0) { // landed on zero
            clicks += 1;
        } else if (rotations[i] > 0) { // going right
            if (dial != 0 && next < dial) {
                clicks += 1;
            }
        } else if (rotations[i] < 0) { // going left
            if (dial != 0 && next > dial) {
                clicks += 1;
            }
        }
            
        fprintf(stdout, "Rotation %d: from %d to %d clicks zero %d times\n", rotations[i], dial, next, clicks);
        total_clicks += clicks;
        dial = next;
        if (dial == 0) {
            ++zeros;
        }
    }
    fprintf(stdout, "Final dial position: %d\n", dial);
    fprintf(stdout, "Number of times dial hit zero: %d\n", zeros);
    fprintf(stdout, "Total clicks: %d\n", total_clicks);

    free(buf);
    free(rotations);
    return 0;
}
