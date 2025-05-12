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
Task extractMin(MinHeap* heap) {
    // Ambil tugas dengan days_left terkecil dan pindahkan tugas terakhir ke root
    Task min = heap->tasks[0];
    heap->tasks[0] = heap->tasks[--heap->size];
    // Atur ulang heap agar tetap memenuhi sifat min-heap
    int i = 0;
    while (1) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < heap->size && heap->tasks[left].days_left < heap->tasks[smallest].days_left)
            smallest = left;
        if (right < heap->size && heap->tasks[right].days_left < heap->tasks[smallest].days_left)
            smallest = right;
        if (smallest == i) break;
        Task temp = heap->tasks[i];
        heap->tasks[i] = heap->tasks[smallest];
        heap->tasks[smallest] = temp;
        i = smallest;
    }
    return min;
}
void rebuildMinHeap(MinHeap* heap, HashTable* ht) {
    // Kosongkan heap dan bangun ulang dengan memasukkan semua tugas dari hash table
    heap->size = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->tasks[i] != NULL) {
            heapInsert(heap, *ht->tasks[i]);
        }
    }
}
void displayByPriority(SiPeTuK* system) {
    // Cek apakah ada tugas yang belum selesai
    if (system->incomplete_tasks.size == 0) {
        printf("\nTidak ada tugas yang belum selesai.\n");
        return;
    }
int k;
    printf("Pilih jumlah tugas mendesak yang ingin ditampilkan:\n");
    printf("1. Top 3\n");
    printf("2. Top 5\n");
    printf("3. Top 10\n");
    printf("Pilih opsi (1-3): ");
    scanf("%d", &k);
    if (k == 1) k = 3;
    else if (k == 2) k = 5;
    else if (k == 3) k = 10;
    else {
        printf("Opsi tidak valid.\n");
        k = 3;
    }

    // Buat heap sementara untuk menampilkan tugas berdasarkan prioritas
    MinHeap temp_heap;
    initMinHeap(&temp_heap, system->priority_heap.size);
    for (int i = 0; i < system->priority_heap.size; i++) {
        heapInsert(&temp_heap, system->priority_heap.tasks[i]);
    }
printf("\n+-------------+--------------------+--------------------+----------------------------------+\n");
    printf("| Sisa Hari   | Mata Kuliah        | Nama Tugas        | Catatan                          |\n");
    printf("+-------------+--------------------+--------------------+----------------------------------+\n");
    int displayed = 0;
    // Ambil dan tampilkan tugas dari heap sementara hingga batas yang diminta
    while (temp_heap.size > 0 && displayed < k) {
        Task task = extractMin(&temp_heap);
        printf("| %-11d | %-18s | %-18s | %-32s |\n",
               task.days_left, task.course, task.name, task.note);
        displayed++;
    }
    printf("+-------------+--------------------+--------------------+----------------------------------+\n");

    free(temp_heap.tasks);
}
