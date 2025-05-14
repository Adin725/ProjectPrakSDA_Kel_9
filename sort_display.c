#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sipe_tuk.h" 

// Fungsi merge untuk merge sort
void merge(Task* arr, int left, int mid, int right) 
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Task* L = malloc(n1 * sizeof(Task));
    Task* R = malloc(n2 * sizeof(Task));
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (strcmp(L[i].course, R[j].course) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L);
    free(R);
}

// Merge sort berdasarkan nama mata kuliah
void mergeSort(Task* arr, int left, int right) 
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
// Binary search berdasarkan nama mata kuliah
int binarySearch(Task* arr, int left, int right, char* course) 
{
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(arr[mid].course, course);
        if (cmp == 0) return mid;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Menampilkan semua tugas berdasarkan status: "done" atau "undone"
void displayTasks(HashTable* ht, const char* type) 
{
    if (ht->size == 0) {
        printf("\nTidak ada tugas %s.\n", type);
        return;
    }

    Task* arr = malloc(ht->size * sizeof(Task));
    int count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->tasks[i] != NULL) {
            arr[count++] = *ht->tasks[i];
        }
    }

    mergeSort(arr, 0, count - 1);

    printf("\n+-----+--------------------+--------------------+----------------------------------+\n");
    printf("| ID  | Mata Kuliah        | Nama Tugas         | Catatan                          |\n");
    printf("+-----+--------------------+--------------------+----------------------------------+\n");
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-18s | %-18s | %-32s |\n",
               arr[i].task_id, arr[i].course, arr[i].name, arr[i].note);
    }
    printf("+-----+--------------------+--------------------+----------------------------------+\n");

    free(arr);
}

// Menampilkan tugas-tugas yang diurutkan berdasarkan nama mata kuliah
void displayBySubject(SiPeTuK* system) {
    char course[50];
    printf("Masukkan nama mata kuliah: ");
    scanf(" %[^\n]", course);

    int count = 0;
    Task* arr = malloc(system->incomplete_tasks.size * sizeof(Task));
    for (int i = 0; i < HASH_SIZE; i++) {
        if (system->incomplete_tasks.tasks[i] != NULL && strcmp(system->incomplete_tasks.tasks[i]->course, course) == 0) {
            arr[count++] = *system->incomplete_tasks.tasks[i];
        }
    }

    if (count == 0) {
        printf("\nTidak ada tugas untuk mata kuliah %s.\n", course);
        free(arr);
        return;
    }

    mergeSort(arr, 0, count - 1);

    printf("\nDaftar Tugas Mata Kuliah %s\n", course);
    printf("+-----+--------------------+----------------------------------+\n");
    printf("| ID  | Nama Tugas         | Catatan                          |\n");
    printf("+-----+--------------------+----------------------------------+\n");
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-18s | %-32s |\n",
               arr[i].task_id, arr[i].name, arr[i].note);
    }
    printf("+-----+--------------------+----------------------------------+\n");

    free(arr);
}
