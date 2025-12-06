#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include "../common.h"

unsigned long part1(char **lines, size_t line_count);
unsigned long part2(char **lines, size_t line_count);
unsigned long long largest_joltage(char *bank, int battery_count);

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day03/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);

    size_t line_count = 0;
    char **lines = split(buf, '\n', &line_count);

    printf("Part 1: %lu\n", part1(lines, line_count));
    printf("Part 2: %lu\n", part2(lines, line_count));
    free (buf);
}

unsigned long long largest_joltage(char *bank, int battery_count) {
    if (battery_count == 0) return 0;
    int batteries_to_ignore = battery_count - 1;
    
    char next_battery = '0';
    int next_battery_pos = 0;
    for (unsigned int pos = 0; pos < strlen(bank) - batteries_to_ignore; pos++) {
        if (bank[pos] > next_battery) {
            next_battery = bank[pos];
            next_battery_pos = pos;
        }
    }

    unsigned long long joltage = pow(10, battery_count - 1) * ((unsigned long long) next_battery - '0');

    return joltage + largest_joltage(&bank[next_battery_pos + 1], battery_count - 1);
}

unsigned long part1(char **lines, size_t line_count) {
    unsigned long part1_joltage = 0;
    for (size_t i = 0; i < line_count; i++) {
        unsigned long long bank_joltage = largest_joltage(lines[i], 2);
        part1_joltage += bank_joltage;
    }

    return part1_joltage;
}

unsigned long part2(char **lines, size_t line_count) {
    unsigned long long part2_joltage = 0;
    for (size_t i = 0; i < line_count; i++) {
        unsigned long long bank_joltage = largest_joltage(lines[i], 12);
        // printf("part 2 %s: %llu\n", lines[i], bank_joltage);
        part2_joltage += bank_joltage;
    }

    return part2_joltage;
}