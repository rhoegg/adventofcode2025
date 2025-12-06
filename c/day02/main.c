#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../common.h"

unsigned long part1(unsigned long start, unsigned long end);
unsigned long part2(unsigned long start, unsigned long end);

unsigned long invalid_ids[4000];
int found_count = 0;


int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day02/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);

    size_t range_count;
    char **range_parts = split(buf, ',', &range_count);

    unsigned long part1_total = 0;
    unsigned long part2_total = 0;
    for (size_t i = 0; i < range_count; i++) {
        printf("%s\n", range_parts[i]);
        unsigned long start, end;
        
        sscanf(range_parts[i], "%lu-%lu", &start, &end);
        // printf("Start: %lu\n", start);
        // printf("End: %lu\n", end);

        part1_total += part1(start, end);
        part2_total += part2(start, end);
    }
    
    printf("Part 1: %lu\n", part1_total);
    printf("Part 2: %lu\n", part2_total);

    free(range_parts);
    free(buf);
}

unsigned long part1(unsigned long start, unsigned long end) {
    unsigned long possible_elf_start;
    if (count_digits(start) % 2 == 1) {
        // start number is 10^len
        possible_elf_start = pow(10, count_digits(start));
    } else {
        possible_elf_start = start;
    }
    possible_elf_start /= pow(10, count_digits(possible_elf_start)/2);
    unsigned long possible_elf_end = end / pow(10, count_digits(end)/2);
    // printf("checking from %lu to %lu\n", possible_elf_start, possible_elf_end);

    unsigned long range_total = 0;
    for (unsigned long elf_pattern = possible_elf_start; elf_pattern <= possible_elf_end; elf_pattern++) {
        unsigned long invalid_id = elf_pattern * pow(10, count_digits(elf_pattern)) + elf_pattern;
        if (invalid_id < start) {
            //printf("skipping id %lu\n", invalid_id);
        }
        else if (invalid_id <= end) {
            //printf("counting invalid id %lu\n", invalid_id);
            range_total += invalid_id;
        } else {
            break;
        }
    }
    return range_total;
}

unsigned long part2(unsigned long start, unsigned long end) {
    unsigned long possible_elf_end = end / pow(10, count_digits(end)/2);
    unsigned long range_total = 0;
    int start_digits = count_digits(start);
    int end_digits = count_digits(end);
    printf("part 2 checking from %lu to %lu\n", 1, possible_elf_end);
    for (unsigned long elf_pattern = 1; elf_pattern <= possible_elf_end; elf_pattern++) {
        int pattern_digits = count_digits(elf_pattern);
        if (start_digits % pattern_digits == 0) {
            int repetitions = start_digits / pattern_digits;
            unsigned long invalid_id = elf_pattern;
            for (int i = 1; i < repetitions; i++) {
                invalid_id = invalid_id * pow(10, pattern_digits) + elf_pattern;
            }
            if (invalid_id >= start && invalid_id <= end && count_digits(invalid_id) > count_digits(elf_pattern)) {
                int new = 1; // true
                for (int i = 0; i < found_count; i++) {
                    if (invalid_ids[i] == invalid_id) {
                        new = 0;
                    }
                }
                if (new) {
                    invalid_ids[found_count++] = invalid_id;
                    range_total += invalid_id;
                    printf("Found invalid id %lu\n", invalid_id);
                }
            }
        }
        if (end_digits > start_digits && end_digits % pattern_digits == 0) {
            int repetitions = end_digits / pattern_digits;
            unsigned long invalid_id = elf_pattern;
            for (int i = 1; i < repetitions; i++) {
                invalid_id = invalid_id * pow(10, pattern_digits) + elf_pattern;
            }
            if (invalid_id >= start && invalid_id <= end && count_digits(invalid_id) > count_digits(elf_pattern)) {
                int new = 1; // true
                for (int i = 0; i < found_count; i++) {
                    if (invalid_ids[i] == invalid_id) {
                        new = 0;
                    }
                }

                if (new) {
                    invalid_ids[found_count++] = invalid_id;
                    range_total += invalid_id;
                    printf("Found invalid id %lu\n", invalid_id);
                }
            }
        }
    }
    return range_total;
}