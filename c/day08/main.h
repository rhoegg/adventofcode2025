#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>

typedef struct JunctionBox JunctionBox;
struct JunctionBox {
    int x, y, z;
    // JunctionBox *connected[1024]; // only if I need the wires
    // size_t connected_count;
};

typedef struct {
    JunctionBox *boxes[1024];
    size_t box_count;
} Circuit;

typedef struct {
    JunctionBox *a, *b;
    unsigned long long distance;
} Pair;


typedef struct {
    Pair *array;
    size_t size;    
    size_t capacity;
} PairHeap;

void parse_input(char* input, size_t *box_count);
unsigned long long distance_sq(JunctionBox a, JunctionBox b);
PairHeap *make_heap(Pair *pairs);

#endif // MAIN_H