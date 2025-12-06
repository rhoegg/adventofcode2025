#include <stdlib.h>
#include <strings.h>
#include "../common.h"

unsigned long part1(char **lines, int line_count);
unsigned long largest_joltage(char *bank, int battery_count);

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
    free (buf);
}

unsigned long largest_joltage(char *bank, int battery_count) {
    if (battery_count == 0) return 0;
    int batteries_to_ignore = battery_count - 1;
    
    char next_battery = '0';
    char next_battery_pos = 0;
    for (unsigned int pos = 0; pos < strlen(bank) - batteries_to_ignore; pos++) {
        if (bank[pos] > next_battery) {
            next_battery = bank[pos];
            next_battery_pos = pos;
        }
    }

    long joltage = next_battery - '0';
    if (battery_count == 1) {
        return joltage;
    }

    return 10 * joltage + largest_joltage(&bank[next_battery_pos + 1], battery_count - 1);
}

unsigned long part1(char **lines, int line_count) {
    unsigned long part1_joltage = 0;
    for (size_t i = 0; i < line_count; i++) {
        char first_battery = lines[i][0];
        int first_battery_pos = 0;
        for (unsigned int pos = 0; pos < strlen(lines[i]) - 1; ++pos) {
            if (lines[i][pos] > first_battery) {
                first_battery = lines[i][pos];
                first_battery_pos = pos;
            }
        }
        char second_battery = '0';
        for (unsigned int pos = first_battery_pos + 1; pos < strlen(lines[i]); ++pos) {
            if (lines[i][pos] > second_battery) {
                second_battery = lines[i][pos];
            }
        }
        unsigned int bank_joltage = 10 * (first_battery - '0') + (second_battery - '0');
        part1_joltage += bank_joltage;
    }

    return part1_joltage;
}

