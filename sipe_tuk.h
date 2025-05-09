
#ifndef SIPE_TUK_H
#define SIPE_TUK_H

#include <time.h>

typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int task_id;
    char name[50];
    char course[50];
    char deadline[11];
    char note[100];
    int days_left;
} Task;

typedef struct StackNode {
    Task task;
    struct StackNode* next;
} StackNode;

typedef struct {
    Task* tasks;
    int size;
    int capacity;
} MinHeap;

#define HASH_SIZE 1000
typedef struct {
    Task* tasks[HASH_SIZE];
    int size;
} HashTable;

typedef struct {
    HashTable incomplete_tasks;
    HashTable completed_tasks;
    MinHeap priority_heap;
    StackNode* undo_stack;
    Date current_date;
    int id_counter;
} SiPeTuK;

// Deklarasi fungsi dari hash_table.c
void initHashTable(HashTable* ht);
void addTaskToHashTable(HashTable* ht, Task task);
Task* findTaskInHashTable(HashTable* ht, int task_id);
void removeTaskFromHashTable(HashTable* ht, int task_id, StackNode** stack);
void addTask(SiPeTuK* system);
void deleteTask(SiPeTuK* system);
void markCompleted(SiPeTuK* system);

// Deklarasi fungsi dari heap.c
void initMinHeap(MinHeap* heap, int capacity);
void heapInsert(MinHeap* heap, Task task);
Task extractMin(MinHeap* heap);
void rebuildMinHeap(MinHeap* heap, HashTable* ht);
void displayByPriority(SiPeTuK* system);

// Deklarasi fungsi dari stack_and_main.c
void initStack(StackNode** stack);
void pushStack(StackNode** stack, Task task);
Task popStack(StackNode** stack);
void undoDelete(SiPeTuK* system);
SiPeTuK* initSiPeTuK();
void freeSystem(SiPeTuK* system);
int calculateDaysLeft(Date current, char* deadline); 

// Deklarasi fungsi dari sort_display.c
void merge(Task* arr, int left, int mid, int right);
void mergeSort(Task* arr, int left, int right);
int binarySearch(Task* arr, int left, int right, char* course);
void displayTasks(HashTable* ht, const char* type);
void displayBySubject(SiPeTuK* system);

#endif
