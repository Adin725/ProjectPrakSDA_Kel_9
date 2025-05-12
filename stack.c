#include <stdio.h>
#include <stdlib.h>
#include "sipe_tuk.h"

// Fungsi untuk menghapus tugas dari hash table dan menyimpannya ke undo stack
void removeTaskFromHashTable(HashTable* table, int task_id, StackNode** undo_stack) {
    int index = hashFunction(task_id);  // Cari index di hash table
    TaskNode* current = table->buckets[index];
    TaskNode* prev = NULL;

    while (current != NULL) {
        if (current->task.task_id == task_id) {
            // Simpan tugas ke undo stack sebelum dihapus
            pushStack(undo_stack, current->task);

            // Hapus tugas dari hash table (linked list)
            if (prev == NULL) {
                table->buckets[index] = current->next;  // Node pertama
            } else {
                prev->next = current->next;  // Menghubungkan ke node berikutnya
            }

            free(current);  // Bebaskan memori untuk node
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Tugas dengan ID %d tidak ditemukan di hash table.\n", task_id);
}


// Inisialisasi stack kosong
void initStack(StackNode** stack) {
    *stack = NULL;
}

// Menambahkan task ke atas stack
void pushStack(StackNode** stack, Task task) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (new_node == NULL) {
        fprintf(stderr, "Gagal mengalokasikan memori untuk stack.\n");
        exit(EXIT_FAILURE);
    }
    new_node->task = task;
    new_node->next = *stack;
    *stack = new_node;
}

// Menghapus task dari atas stack dan mengembalikannya
Task popStack(StackNode** stack) {
    if (*stack == NULL) {
        Task empty_task = {0};  // task_id = 0 sebagai penanda stack kosong
        return empty_task;
    }

    StackNode* temp = *stack;
    Task popped_task = temp->task;
    *stack = temp->next;
    free(temp);
    return popped_task;
}

// Melakukan undo terhadap penghapusan task
void undoDelete(SiPeTuK* system) {
    if (system->undo_stack == NULL) {
        printf("Tidak ada operasi yang dapat di-undo.\n");
        return;
    }

    Task task_to_undo = popStack(&system->undo_stack);
    if (task_to_undo.task_id == 0) {
        printf("Undo gagal: tidak ada tugas yang tersedia.\n");
        return;
    }

    addTaskToHashTable(&system->incomplete_tasks, task_to_undo);
    heapInsert(&system->priority_heap, task_to_undo);
    printf("Tugas ID %d telah dipulihkan.\n", task_to_undo.task_id);
}
