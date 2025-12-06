#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../common.h"

unsigned long part1(unsigned long start, unsigned long end);

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
    char *separator = NULL;
    for (size_t i = 0; i < range_count; i++) {
        printf("%s\n", range_parts[i]);
        unsigned long start, end;
        separator = strchr(range_parts[i], '-');
        int start_len = separator - range_parts[i];
        int end_len = strlen(range_parts[i]) - start_len - 1;
        
        sscanf(range_parts[i], "%lu-%lu", &start, &end);
        // printf("Start: %lu\n", start);
        // printf("End: %lu\n", end);
        // take the first "half" of the start
        printf("lengths: %d - %d\n", start_len, end_len);
        if (start_len % 2 == 1 && end_len == start_len) {
            // odd number of digits both sides
            continue;
        }

        part1_total += part1(start, end);
    }
    
    printf("Part 1: %lu\n", part1_total);

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
        printf("testing elf pattern %lu\n", invalid_id);
        if (invalid_id < start) {
            printf("skipping id %lu\n", invalid_id);
        }
        else if (invalid_id <= end) {
            printf("counting invalid id %lu\n", invalid_id);
            range_total += invalid_id;
        } else {
            break;
        }
    }
    return range_total;
}