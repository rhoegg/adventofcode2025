#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

void swap(Trial *a, Trial *b) {
    Trial tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify(TrialHeap *heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size &&
        heap->trials[left].len_buttons < heap->trials[smallest].len_buttons) {
            smallest = left;
        }
    if (right < heap->size &&
        heap->trials[right].len_buttons < heap->trials[smallest].len_buttons) {
            smallest = right;
        }
    if (smallest != i) {
        swap(&heap->trials[i], &heap->trials[smallest]);
        heapify(heap, smallest);
    }
}

void insert_heap(TrialHeap *heap, Trial value) {
    if (heap->size == heap->capacity) {
        size_t new_capacity = heap->capacity * 2;
        Trial *new_array = realloc(heap->trials, new_capacity * sizeof(Trial));

        if (!new_array) {
            fprintf(stderr, "out of memory growing heap");
            exit(EXIT_FAILURE);
        }
        heap->trials = new_array;
        heap->capacity = new_capacity;
    }

    heap->size++;
    int i = heap->size - 1;
    heap->trials[i] = value;
    while (i != 0 && heap->trials[(i - 1) / 2].len_buttons > heap->trials[i].len_buttons) {
        swap(&heap->trials[i], &heap->trials[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Trial extract_heap_min(TrialHeap *heap) {
    if (heap->size <= 0) {
        fprintf(stderr, "can't extract from an empty heap");
        exit(EXIT_FAILURE);
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->trials[0];
    }
    Trial root = heap->trials[0];
    heap->trials[0] = heap->trials[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return root;
}

