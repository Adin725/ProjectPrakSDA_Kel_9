#include <stdio.h>
#include <stdlib.h>
#include "sipe_tuk.h"

void initMinHeap(MinHeap* heap, int capacity) {
    // Inisialisasi heap dengan kapasitas awal dan alokasi memori untuk array tasks
    heap->size = 0;
    heap->capacity = capacity;
    heap->tasks = malloc(capacity * sizeof(Task));
}
