#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sipe_tuk.h"

// Menghitung sisa hari dari tanggal saat ini ke tenggat waktu
int calculateDaysLeft(Date current, char* deadline) {
    int d1, m1, y1;
    sscanf(deadline, "%d_%d_%d", &d1, &m1, &y1);
    int current_days = current.year * 365 + current.month * 30 + current.day;
    int deadline_days = y1 * 365 + m1 * 30 + d1;
    return deadline_days - current_days;
}

// Inisialisasi sistem SiPeTuK
SiPeTuK* initSiPeTuK() {
    SiPeTuK* system = malloc(sizeof(SiPeTuK));
    initHashTable(&system->incomplete_tasks);
    initHashTable(&system->completed_tasks);
    initMinHeap(&system->priority_heap, 100);
    initStack(&system->undo_stack);

    // Set tanggal saat ini menggunakan waktu sistem
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    system->current_date.day = t->tm_mday;
    system->current_date.month = t->tm_mon + 1;
    system->current_date.year = t->tm_year + 1900;
    system->id_counter = 0;

    // Baca tugas yang belum selesai dari file, tambahkan ke Hash Table dan Min-Heap
    FILE* file_incomplete = fopen("incomplete_tasks.txt", "r");
    if (file_incomplete == NULL) {
        file_incomplete = fopen("incomplete_tasks.txt", "w");
        fclose(file_incomplete);
        printf("File incomplete_tasks.txt dibuat.\n");
    } else {
        char line[200];
        while (fgets(line, sizeof(line), file_incomplete)) {
            Task task;
            sscanf(line, "%d,%49[^,],%49[^,],%10[^,],%99[^\n]", &task.task_id, task.name, task.course, task.deadline, task.note);
            task.days_left = calculateDaysLeft(system->current_date, task.deadline);
            addTaskToHashTable(&system->incomplete_tasks, task);
            heapInsert(&system->priority_heap, task);
            if (task.task_id > system->id_counter) system->id_counter = task.task_id;
        }
        fclose(file_incomplete);
    }

    // Baca tugas yang selesai dari file, tambahkan ke Hash Table
    FILE* file_completed = fopen("completed_tasks.txt", "r");
    if (file_completed == NULL) {
        file_completed = fopen("completed_tasks.txt", "w");
        fclose(file_completed);
        printf("File completed_tasks.txt dibuat.\n");
    } else {
        char line[200];
        while (fgets(line, sizeof(line), file_completed)) {
            Task task;
            sscanf(line, "%d,%49[^,],%49[^,],%10[^,],%99[^\n]", &task.task_id, task.name, task.course, task.deadline, task.note);
            task.days_left = 0;
            addTaskToHashTable(&system->completed_tasks, task);
            if (task.task_id > system->id_counter) system->id_counter = task.task_id;
        }
        fclose(file_completed);
    }

    system->id_counter++;
    return system;
}

// Simpan data ke file dan bebaskan memori sebelum keluar
void freeSystem(SiPeTuK* system) {
    // Tulis ulang tugas ke file sebelum membebaskan memori
    FILE* file_incomplete = fopen("incomplete_tasks.txt", "w");
    FILE* file_completed = fopen("completed_tasks.txt", "w");
    for (int i = 0; i < HASH_SIZE; i++) {
        if (system->incomplete_tasks.tasks[i] != NULL) {
            fprintf(file_incomplete, "%d,%s,%s,%s,%s\n", system->incomplete_tasks.tasks[i]->task_id,
                    system->incomplete_tasks.tasks[i]->name, system->incomplete_tasks.tasks[i]->course,
                    system->incomplete_tasks.tasks[i]->deadline, system->incomplete_tasks.tasks[i]->note);
            free(system->incomplete_tasks.tasks[i]);
        }
        if (system->completed_tasks.tasks[i] != NULL) {
            fprintf(file_completed, "%d,%s,%s,%s,%s\n", system->completed_tasks.tasks[i]->task_id,
                    system->completed_tasks.tasks[i]->name, system->completed_tasks.tasks[i]->course,
                    system->completed_tasks.tasks[i]->deadline, system->completed_tasks.tasks[i]->note);
            free(system->completed_tasks.tasks[i]);
        }
    }
    fclose(file_incomplete);
    fclose(file_completed);

    // Bebaskan memori Stack dan Min-Heap
    while (system->undo_stack) {
        StackNode* temp = system->undo_stack;
        system->undo_stack = temp->next;
        free(temp);
    }
    free(system->priority_heap.tasks);
    free(system);
}

// Fungsi utama: Menjalankan menu SiPeTuK
int main() {
    SiPeTuK* system = initSiPeTuK(); // Inisialisasi sistem

    printf("=== SiPeTuK: Sistem Pengelola Tugas Kuliah ===\n");
    printf("Tanggal Saat Ini: %02d-%02d-%04d\n", 
           system->current_date.day, system->current_date.month, system->current_date.year);

    int choice;
    while (1) { 
        printf("\n=== Menu SiPeTuK ===\n");
        printf("1. Tambah Tugas\n");
        printf("2. Lihat Tugas Belum Selesai\n");
        printf("3. Lihat Tugas Selesai\n");
        printf("4. Hapus Tugas\n");
        printf("5. Undo Penghapusan\n");
        printf("6. Tandai Selesai\n");
        printf("7. Lihat Tugas Berdasarkan Mata Kuliah\n");
        printf("8. Lihat Tugas Berdasarkan Prioritas\n");
        printf("9. Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1: addTask(system); break;
            case 2: displayTasks(&system->incomplete_tasks, "Belum Selesai"); break;
            case 3: displayTasks(&system->completed_tasks, "Selesai"); break;
            case 4: deleteTask(system); break;
            case 5: undoDelete(system); break;
            case 6: markCompleted(system); break;
            case 7: displayBySubject(system); break;
            case 8: displayByPriority(system); break;
            case 9: freeSystem(system); printf("Program selesai.\n"); return 0; 
            default: printf("Opsi tidak valid!\n");
        }
    }
}
