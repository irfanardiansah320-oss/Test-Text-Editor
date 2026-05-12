#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_cursor.h" 
#include "linkedlist.h"

// =========================
// EDITOR BERBASIS LINKED LIST
// =========================

void tampilkanNode(Node *head) {
    if (head == NULL) {
        printf("(Kosong)\n");
        return;
    }
    Node *curr = head;
    int no = 1;
    while (curr != NULL) {
        printf("%3d. %s\n", no++, curr->line);
        curr = curr->next;
    }
}

    void simpanKeFile(Node *head, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("\033[1;31mGagal menyimpan file!\033[0m\n");
        return;
    }
    Node *curr = head;
    while (curr != NULL) {
        fprintf(f, "%s", curr->line);
        if (curr->next != NULL) fprintf(f, "\n");
        curr = curr->next;
    }
    fclose(f);
    printf("\033[1;32mFile '%s' telah disimpan.\033[0m\n", filename);
}

    void editLinkedList(Node **head, const char *filename) {
    int pilihan;
    do {
        system("cls");
        printf("\n\033[1;36m=== EDIT FILE (LINKED LIST) ===\033[0m\n");
        printf("Isi file saat ini:\n");
        printf("----------------------------------------\n");
        tampilkanNode(*head);
        printf("----------------------------------------\n");
        printf("\n\033[1;33mMenu Edit:\033[0m\n");
        printf("1. Edit baris tertentu\n");
        printf("2. Tambah baris baru di akhir\n");
        printf("3. Hapus baris\n");
        printf("4. Simpan & Keluar\n");
        printf("5. Keluar tanpa simpan\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);
        getchar(); // bersihkan newline

        switch (pilihan) {
            case 1: {
                int nomor;
                printf("Masukkan nomor baris yang akan diedit: ");
                scanf("%d", &nomor);
                getchar();
                Node *curr = getNodeAt(*head, nomor - 1);
                if (curr == NULL) {
                    printf("Baris tidak valid!\n");
                } else {
                    printf("Baris lama: %s\n", curr->line);
                    printf("Masukkan baris baru: ");
                    fgets(curr->line, MAX_COLS, stdin);
                    curr->line[strcspn(curr->line, "\n")] = '\0';
                }
                break;
            }
            case 2: {
                char newLine[MAX_COLS];
                printf("Masukkan baris baru: ");
                fgets(newLine, MAX_COLS, stdin);
                newLine[strcspn(newLine, "\n")] = '\0';
                appendNode(head, newLine);
                printf("Baris berhasil ditambahkan.\n");
                break;
            }
            case 3: {
                int nomor;
                printf("Masukkan nomor baris yang akan dihapus: ");
                scanf("%d", &nomor);
                getchar();
                if (nomor == 1) {
                    Node *temp = *head;
                    *head = (*head)->next;
                    free(temp);
                } else {
                    Node *prev = getNodeAt(*head, nomor - 2);
                    if (prev == NULL || prev->next == NULL) {
                        printf("Baris tidak valid!\n");
                    } else {
                        Node *temp = prev->next;
                        prev->next = temp->next;
                        free(temp);
                    }
                }
                printf("Baris dihapus.\n");
                break;
            }
            case 4:
                simpanKeFile(*head, filename);
                return; // keluar dari editor
            case 5:
                printf("Keluar tanpa menyimpan.\n");
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
    } while (1);
}

// =========================
// FUNGSI openFile (sesuai prototype di rere1.h)
// =========================
void openFile() {
    char filename[100];
    printf("\n\033[1;36m=== OPEN FILE ===\033[0m\n");
    printf("Masukkan nama file: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("\033[1;31mFile '%s' tidak ditemukan!\033[0m\n", filename);
        return;
    }

    // Baca file ke dalam linked list
    Node *head = NULL;
    char buffer[MAX_COLS];
    while (fgets(buffer, MAX_COLS, file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // hapus newline
        appendNode(&head, buffer);
    }
    fclose(file);

    // Tampilkan isi file
    printf("\033[1;33mIsi file:\033[0m\n");
    printf("----------------------------------------\n");
    tampilkanNode(head);
    printf("----------------------------------------\n");

    char choice;
    printf("\nApakah kamu ingin mengedit file ini? (y/n): ");
    scanf(" %c", &choice);
    getchar();

    if (choice == 'y' || choice == 'Y') {
        editLinkedList(&head, filename);
    } else {
        printf("File tidak diedit.\n");
    }

    // Bebaskan memori linked list
    freeList(head);
}