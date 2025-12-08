#include <stdlib.h>
#include <stdio.h>
#include "main.h"

void swap_pairs(Pair *a, Pair *b) {
    Pair temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_pairs(PairHeap *heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size &&
        heap->array[left].distance < heap->array[smallest].distance) {
            smallest = left;
        }
    if (right < heap->size &&
        heap->array[right].distance < heap->array[smallest].distance) {
            smallest = right;
        }
    if (smallest != i) {
        swap_pairs(&heap->array[i], &heap->array[smallest]);
        heapify_pairs(heap, smallest); // follow it down the tree
    }
}

void build_pair_heap(PairHeap *heap) {
    int n = heap->size;
    for (int i = (n - 1) / 2; i >= 0; --i) {
        heapify_pairs(heap, i);
    }
}

void insert_pair_heap(PairHeap *heap, Pair value) {
    if (heap->size == heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        Pair *new_array = realloc(heap->array, new_capacity * sizeof(Pair));
        
        if (!new_array) {
            fprintf(stderr, "out of memory growing heap");
            exit(EXIT_FAILURE);
        }
        heap->array = new_array;
        heap->capacity = new_capacity;
    }

    heap->size++;
    int i = heap->size - 1;
    heap->array[i] = value;
    // fix it
    while (i != 0 && heap->array[(i - 1) / 2].distance > heap->array[i].distance) {
        swap_pairs(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Pair extract_pair_heap_min(PairHeap *heap) {
    if (heap->size <= 0) {
        fprintf(stderr, "can't extract from an empty heap\n");
        exit(EXIT_FAILURE);
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }
    Pair root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapify_pairs(heap, 0);
    return root;
}