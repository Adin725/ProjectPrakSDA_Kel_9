#include <stdio.h>
#include <stdlib.h>
#include "sipe_tuk.h" 

// Fungsi untuk menginisialisasi stack undo
void initStack(StackNode** stack) {
    *stack = NULL;
}
// Fungsi untuk push tugas ke dalam stack undo
void pushStack(StackNode** stack, Task task) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    new_node->task = task;
    new_node->next = *stack;
    *stack = new_node;
}
// Fungsi untuk pop tugas dari stack undo
Task popStack(StackNode** stack) {
    if (*stack == NULL) {
        Task empty_task = {0};
        return empty_task;
    }
    StackNode* temp = *stack;
    Task popped_task = temp->task;
    *stack = temp->next;
    free(temp);
    return popped_task;
}
// Fungsi untuk undo operasi penghapusan tugas
void undoDelete(SiPeTuK* system) {
    if (system->undo_stack == NULL) {
        printf("Tidak ada operasi yang dapat di-undo.\n");
        return;
    }
    Task task_to_undo = popStack(&system->undo_stack);
    addTaskToHashTable(&system->incomplete_tasks, task_to_undo);
    heapInsert(&system->priority_heap, task_to_undo);
    printf("Tugas ID %d telah dipulihkan.\n", task_to_undo.task_id);
}
