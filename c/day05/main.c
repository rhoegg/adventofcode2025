#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "../common.h"

struct Range {
    unsigned long long from;
    unsigned long long to;
};

struct CafeteriaDatabase {
    struct Range ranges[512];
    unsigned long long ids[1024];
    size_t range_count;
    size_t id_count;
};

struct CafeteriaDatabase parse_input(char *buf);

void merge_ranges(struct CafeteriaDatabase *d);

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day05/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);
    struct CafeteriaDatabase d = parse_input(buf);

    merge_ranges(&d); // needed for part 2
    printf("merged ranges\n");
    for (size_t i = 0; i < d.range_count; ++i) {
        printf("%llu-%llu\n", d.ranges[i].from, d.ranges[i].to);
    }

    size_t fresh_count = 0;
    
    for (size_t i = 0; i < d.id_count; ++i) {
        bool found = false;
        for (size_t r = 0; r < d.range_count && !found; ++r) {
            if (d.ranges[r].from <= d.ids[i] && d.ranges[r].to >= d.ids[i]) {
                ++fresh_count;
                found = true;
            }
        }
    }

    printf("Part 1: %lu\n", fresh_count);

    unsigned long long fresh_id_count = 0; // for part 2
    for (size_t i = 0; i < d.range_count; i++) {
        fresh_id_count += d.ranges[i].to - d.ranges[i].from + 1;
    }

    printf("Part 2: %llu\n", fresh_id_count);


    free(buf);
}

void merge_ranges(struct CafeteriaDatabase *d) {
    size_t merged_count = 0;
    
    for (size_t i = 0; i < d->range_count; ++i) {
        for (size_t j = 0; j < d->range_count; ++j) {
            if (i == j) continue;
            if (d->ranges[i].from >= d->ranges[j].from && d->ranges[i].from <= d->ranges[j].to) {
                // don't need to check if the end of the range overlaps because we're going to see that when the other range is the outer loop
                struct Range *merge = (i < j) ? &(d->ranges[i]) : &(d->ranges[j]);
                struct Range *reap = (i < j) ? &(d->ranges[j]) : &(d->ranges[i]);
                merge->from = (merge->from < reap->from) ? merge->from : reap->from;
                merge->to = (merge->to < reap->to) ? reap->to : merge->to;
                struct Range *tail = &(d->ranges[d->range_count - 1]);
                if (reap != tail) {
                    *reap = *tail;
                }
                --d->range_count;
                ++merged_count;
            }
        }
    }
    if (merged_count == 0) return;
    printf("merged %lu ranges\n", merged_count);
    merge_ranges(d);
}

struct CafeteriaDatabase parse_input(char *buf) {
    char *parts_separator = strstr(buf, "\n\n");
    *parts_separator = '\0';
    char *ranges_input = buf;
    char *ids_input = parts_separator + 2; // 2 characters after the separator

    struct CafeteriaDatabase d = {};
    char **range_lines = split(ranges_input, '\n', &d.range_count);
    char **id_lines = split(ids_input, '\n', &d.id_count);

    for (size_t i = 0; i < d.range_count; i++) {
        sscanf(range_lines[i], "%llu-%llu", &(d.ranges[i].from), &(d.ranges[i].to));
    }
    for (size_t i = 0; i < d.id_count; i++) {
        d.ids[i] = strtoull(id_lines[i], NULL, 10);
    }
    return d;
}