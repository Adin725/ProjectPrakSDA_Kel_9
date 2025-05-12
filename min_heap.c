#include <stdio.h>
#include <stdlib.h>
#include "sipe_tuk.h"

void initMinHeap(MinHeap* heap, int capacity) {
    // Inisialisasi heap dengan kapasitas awal dan alokasi memori untuk array tasks
    heap->size = 0;
    heap->capacity = capacity;
    heap->tasks = malloc(capacity * sizeof(Task));
}
void heapInsert(MinHeap* heap, Task task) {
    // Jika heap penuh, gandakan kapasitasnya dan realokasi memori
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->tasks = realloc(heap->tasks, heap->capacity * sizeof(Task));
    }
    // Tambahkan tugas baru di akhir heap, lalu naikkan ke posisi yang tepat berdasarkan days_left
    int i = heap->size++;
    while (i > 0 && heap->tasks[(i - 1) / 2].days_left > task.days_left) {
        heap->tasks[i] = heap->tasks[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->tasks[i] = task;
}
