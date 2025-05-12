#include <stdio.h>
#include <stdlib.h>
#include "header.h"

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
