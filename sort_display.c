#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sipe_tuk.h" 

// Fungsi merge untuk merge sort
void merge(Task* arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Alokasi dinamis untuk array L dan R
    Task** L = (Task**)malloc(n1 * sizeof(Task*));
    Task** R = (Task**)malloc(n2 * sizeof(Task*));

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (strcmp(L[i]->course, R[j]->course) <= 0) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }

    free(L);
    free(R);
}

// Merge sort berdasarkan nama mata kuliah
void mergeSort(Task* arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Binary search berdasarkan nama mata kuliah
int binarySearch(Task* arr[], int left, int right, char* course) {
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(arr[mid]->course, course);
        if (cmp == 0) return mid;
        else if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// Menampilkan semua tugas berdasarkan status: "done" atau "undone"
void displayTasks(HashTable* ht, const char* type) {
    printf("\nDaftar tugas (%s):\n", type);
    int found = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht->tasks[i] != NULL) {
            Task* t = ht->tasks[i];
            if (t != NULL) {
                found = 1;
                printf("ID: %d | Nama: %s | MK: %s | Deadline: %s | Catatan: %s\n",
                    t->task_id, t->name, t->course, t->deadline, t->note);
            }
        }
    }
    if (!found) {
        printf("Tidak ada tugas dengan status \"%s\".\n", type);
    }
}

// Menampilkan tugas-tugas yang diurutkan berdasarkan nama mata kuliah
void displayBySubject(SiPeTuK* system) {
    Task* temp[HASH_SIZE];
    int idx = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (system->incomplete_tasks.tasks[i] != NULL) {
            temp[idx++] = system->incomplete_tasks.tasks[i];
        }
    }

    if (idx == 0) {
        printf("Tidak ada tugas untuk ditampilkan.\n");
        return;
    }
