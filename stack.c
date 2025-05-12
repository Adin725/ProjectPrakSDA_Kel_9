#include <stdio.h>
#include <stdlib.h>
#include "sipe_tuk.h"

// Inisialisasi stack kosong
void initStack(StackNode** stack) {
    *stack = NULL;
}

// Menambahkan task ke atas stack
void pushStack(StackNode** stack, Task task) {
    StackNode* new_node = malloc(sizeof(StackNode));
    new_node->task = task;
    new_node->next = *stack;
    *stack = new_node;
}

// Menghapus task dari atas stack dan mengembalikannya
Task popStack(StackNode** stack) {
    if (*stack == NULL) {
        Task empty = {0, "", "", "", "", 0};
        return empty;
    }
    StackNode* temp = *stack;
    Task task = temp->task;
    *stack = (*stack)->next;
    free(temp);
    return task;
}

// Melakukan undo terhadap penghapusan task
void undoDelete(SiPeTuK* system) {
    Task task = popStack(&system->undo_stack);
    if (task.task_id == 0) {
        printf("Tidak ada tugas untuk dikembalikan.\n");
        return;
    }

    task.days_left = calculateDaysLeft(system->current_date, task.deadline);
    addTaskToHashTable(&system->incomplete_tasks, task);
    heapInsert(&system->priority_heap, task);
    printf("Tugas berhasil dikembalikan!\n");
}
