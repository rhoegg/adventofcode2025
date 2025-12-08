#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "../common.h"
#include "main.h"
#include "heap.h"

JunctionBox decoration_project[2048];
size_t box_count;
void part1(PairHeap *heap, Circuit *circuits, size_t circuit_count, int pairs);

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day08/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);
    parse_input(buf, &box_count);

    Pair *pairs = malloc(1000000 * sizeof(Pair));

    PairHeap *heap = make_heap(pairs);

    Circuit *circuits = malloc(box_count * sizeof(Circuit));
    size_t circuit_count = box_count;
    for (size_t i = 0; i < box_count; ++i) {
        circuits[i] = (Circuit) {
            .boxes[0] = &decoration_project[i],
            .box_count = 1,
        };
    }

    part1(heap, circuits, circuit_count, 1000);

    free(circuits);
    free(heap);
    free(pairs);
    free(buf);
}

int compare_circuits_desc(const void *a, const void *b) {
    const Circuit *va = a;
    const Circuit *vb = b;

    if (va->box_count < vb->box_count) return 1;
    if (va->box_count > vb->box_count) return -1;
    return 0;
}

void part1(PairHeap *heap, Circuit *circuits, size_t circuit_count, int pairs) {
    int processed = 0;
    int connection_count = 0;
    while (processed < pairs) {
        Pair p = extract_pair_heap_min(heap);
        processed++;
        int circuit_a = -1, circuit_b = -1;
        for (size_t i = 0; i < circuit_count; ++i) {
            for (size_t j = 0; j < circuits[i].box_count; ++j) {
                if (circuits[i].boxes[j] == p.a) {
                    circuit_a = i;
                }
                if (circuits[i].boxes[j] == p.b) {
                    circuit_b = i;
                }
            }
        }
        if (circuit_a == -1 || circuit_b == -1) {
            printf("p.a = %p, p.b = %p\n", (void*)p.a, (void*)p.b);
            fprintf(stderr, "ERROR: endpoint not found for pair at connection %d (a=%d, b=%d)\n",
                    connection_count + 1, circuit_a, circuit_b);
            exit(EXIT_FAILURE);
        }
        if (circuit_a != circuit_b) {
            // move all boxes b to a
            for (size_t i = 0; i < circuits[circuit_b].box_count; ++i) {
                circuits[circuit_a].boxes[circuits[circuit_a].box_count + i] = circuits[circuit_b].boxes[i];
            }
            // fix box count dest
            circuits[circuit_a].box_count = circuits[circuit_a].box_count + circuits[circuit_b].box_count;
            // replace reap with last
            circuits[circuit_b] = circuits[circuit_count - 1];
            // reduce count
            circuit_count--;
            connection_count++;
        }
        size_t boxes_in_circuits = 0;
        for (size_t i = 0; i < circuit_count; ++i) {
            boxes_in_circuits += circuits[i].box_count;
        }
        if (boxes_in_circuits != box_count) {
            fprintf(stderr, "ERROR: unexpected boxes (%lu != %lu) after %d connections\n", boxes_in_circuits, box_count, connection_count);
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < circuit_count; ++i) {
        printf("%lu ", circuits[i].box_count);
    }
    printf("\n");
    qsort(circuits, circuit_count, sizeof(Circuit), compare_circuits_desc);

    printf("Part 1: %lu*%lu*%lu=%lu\n", circuits[0].box_count, circuits[1].box_count, circuits[2].box_count, 
            circuits[0].box_count * circuits[1].box_count * circuits[2].box_count);
}

PairHeap *make_heap(Pair *pairs) {
    size_t pair_count = 0;
    for (size_t i = 0; i < box_count; ++i) {
        for (size_t j = i + 1; j < box_count; ++j) {
            pairs[pair_count++] = (Pair) {
                .a = &decoration_project[i],
                .b = &decoration_project[j],
                .distance = distance_sq(decoration_project[i], decoration_project[j])
            };
        }
    }
    PairHeap *pair_heap = malloc(sizeof(PairHeap));
    pair_heap->array = pairs;
    pair_heap->capacity = pair_count;
    pair_heap->size = pair_count;
    
    build_pair_heap(pair_heap);
    return pair_heap;
}

unsigned long long distance_sq(JunctionBox a, JunctionBox b) {
    long dx = a.x - b.x;
    long dy = a.y - b.y;
    long dz = a.z - b.z;

    unsigned long long ux = (dx * dx);
    unsigned long long uy = (dy * dy);
    unsigned long long uz = (dz * dz);

    return ux + uy + uz;
}

void parse_input(char* input, size_t *box_count) {
    char **lines = split(input, '\n', box_count);
    for (size_t i = 0; i < *box_count; ++i) {
        decoration_project[i] = (JunctionBox) {};
        sscanf(lines[i], "%d,%d,%d", &(decoration_project[i].x), &(decoration_project[i].y), &(decoration_project[i].z));
    }
}