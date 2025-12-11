#include "main.h"

typedef struct {
    Trial *trials;
    int size;
    int capacity;
} TrialHeap;

void insert_heap(TrialHeap *heap, Trial value);
Trial extract_heap_min(TrialHeap *heap);