#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sipe_tuk.h"

// Hitung indeks berdasarkan ID tugas untuk penyimpanan di Hash Table
int hashFunction(int task_id) {
    return task_id % HASH_SIZE;
}

// Inisialisasi Hash Table dengan size nol dan semua slot kosong
void initHashTable(HashTable* ht) {
    ht->size = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->tasks[i] = NULL;
    }
}

// Tambah tugas baru ke Hash Table dengan penanganan collision
void addTaskToHashTable(HashTable* ht, Task task) {
    int index = hashFunction(task.task_id);
    while (ht->tasks[index] != NULL) {
        index = (index + 1) % HASH_SIZE;
    }
    ht->tasks[index] = malloc(sizeof(Task));
    *ht->tasks[index] = task;
    ht->size++;
}

// Cari tugas di Hash Table berdasarkan ID
Task* findTaskInHashTable(HashTable* ht, int task_id) {
    int index = hashFunction(task_id);
    int original = index;
    do {
        if (ht->tasks[index] != NULL && ht->tasks[index]->task_id == task_id) {
            return ht->tasks[index];
        }
        index = (index + 1) % HASH_SIZE;
    } while (index != original && ht->tasks[index] != NULL);
    return NULL;
}

// Hapus tugas dari Hash Table dan simpan ke Stack untuk undo
void removeTaskFromHashTable(HashTable* ht, int task_id, StackNode** stack) {
    int index = hashFunction(task_id);
    int original = index;
    do {
        if (ht->tasks[index] != NULL && ht->tasks[index]->task_id == task_id) {
            StackNode* new_node = malloc(sizeof(StackNode));
            new_node->task = *ht->tasks[index];
            new_node->next = *stack;
            *stack = new_node;
            free(ht->tasks[index]);
            ht->tasks[index] = NULL;
            ht->size--;
            return;
        }
        index = (index + 1) % HASH_SIZE;
    } while (index != original && ht->tasks[index] != NULL);
}

// Tambah tugas baru 
void addTask(SiPeTuK* system) {
    Task task;
    task.task_id = system->id_counter++;
    printf("Masukkan nama tugas: ");
    scanf(" %[^\n]", task.name);
    printf("Masukkan mata kuliah: ");
    scanf(" %[^\n]", task.course);
    printf("Masukkan tenggat waktu (DD_MM_YYYY): ");
    scanf(" %[^\n]", task.deadline);
    printf("Masukkan catatan: ");
    scanf(" %[^\n]", task.note);
    task.days_left = calculateDaysLeft(system->current_date, task.deadline);
    addTaskToHashTable(&system->incomplete_tasks, task);
    heapInsert(&system->priority_heap, task);
    printf("Tugas berhasil ditambahkan!\n");
}

// Hapus tugas dari sistem berdasarkan ID yang dimasukkan
void deleteTask(SiPeTuK* system) {
    int task_id;
    printf("Masukkan ID tugas yang akan dihapus: ");
    scanf("%d", &task_id);

    if (system->incomplete_tasks.size == 0) {
        printf("Tidak ada tugas untuk dihapus.\n");
        return;
    }

    Task* task = findTaskInHashTable(&system->incomplete_tasks, task_id);
    if (task == NULL) {
        printf("Tugas tidak ditemukan.\n");
        return;
    }

    removeTaskFromHashTable(&system->incomplete_tasks, task_id, &system->undo_stack);
    rebuildMinHeap(&system->priority_heap, &system->incomplete_tasks);
    printf("Tugas berhasil dihapus!\n");
}

// Tandai tugas sebagai selesai dan pindahkan ke daftar selesai
void markCompleted(SiPeTuK* system) {
    int task_id;
    printf("Masukkan ID tugas yang selesai: ");
    scanf("%d", &task_id);

    if (system->incomplete_tasks.size == 0) {
        printf("Tidak ada tugas untuk ditandai selesai.\n");
        return;
    }

    Task* task = findTaskInHashTable(&system->incomplete_tasks, task_id);
    if (task == NULL) {
        printf("Tugas tidak ditemukan.\n");
        return;
    }

    addTaskToHashTable(&system->completed_tasks, *task);
    removeTaskFromHashTable(&system->incomplete_tasks, task_id, &system->undo_stack);
    rebuildMinHeap(&system->priority_heap, &system->incomplete_tasks);
    printf("Tugas ditandai selesai!\n");
}
