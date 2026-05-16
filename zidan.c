#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_cursor.h"
#include "zidan.h"


void findAndReplace() {
    char filename[20];
    char find[100], replace[100];
    char buffer[1000];

    printf("Masukkan nama file: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File tidak ditemukan!\n");
        return;
    }

    printf("Kata yang ingin dicari: ");
    fgets(find, sizeof(find), stdin);
    find[strcspn(find, "\n")] = 0;

    // VALIDASI INPUT KOSONG
    if (strlen(find) == 0) {
        printf("Input tidak boleh kosong!\n");
        fclose(fp);
        return;
    }

    printf("Kata pengganti: ");
    fgets(replace, sizeof(replace), stdin);
    replace[strcspn(replace, "\n")] = 0;

    // VALIDASI PANJANG REPLACE
    if (strlen(replace) > 20) {
        printf("\n[!] Peringatan: kata pengganti maksimal 20 karakter!\n");
        fclose(fp);
        return;
    }

    // LINKED LIST
    Node *head = NULL;
    while (fgets(buffer, sizeof(buffer), fp)) {
        appendNode(&head, buffer);
    }
    fclose(fp);

    // FIND & REPLACE
    int found = 0;
    Node *curr = head;

    while (curr != NULL) {
        char result[1000] = "";
        char temp[1000];
        char *pos;
        char *start = curr->line;

        while ((pos = strstr(start, find)) != NULL) {
            found++;
            
            strncpy(temp, start, pos - start);
            temp[pos - start] = '\0';
            strcat(result, temp);
            strcat(result, replace);
            start = pos + strlen(find);
        }

        strcat(result, start);
        strcpy(curr->line, result);
        curr = curr->next;
    }

    // VALIDASI KATA TIDAK DITEMUKAN
    if (found == 0) {
        printf("Kata tidak ditemukan dalam file!\n");
        freeList(head);
        return;
    }
    // =========================
    // SAVE FILE
    // =========================

    fp = fopen(filename, "w");
    if (!fp) {
        printf("Gagal membuka file!\n");
        freeList(head);
        return;
    }

    curr = head;
    while (curr != NULL) {
        fputs(curr->line, fp);
        curr = curr->next;
    }
    fclose(fp);

    printf("Berhasil replace kata!\n");
    // =========================
    // FREE MEMORY
    // =========================

    freeList(head);
}
void handleCursorMovement(
    int ch,
    int *cursorRow,
    int *cursorCol,
    Node **currentLine
)
{
    // ATAS
    if (ch == 72) {

        if ((*currentLine)->prev != NULL) {

            *currentLine = (*currentLine)->prev;

            (*cursorRow)--;

            int len = strlen((*currentLine)->line);

            if (*cursorCol > len) {

                *cursorCol = len;
            }
        }
    }

    // BAWAH
    else if (ch == 80) {

        if ((*currentLine)->next != NULL) {

            *currentLine = (*currentLine)->next;

            (*cursorRow)++;

            int len = strlen((*currentLine)->line);

            if (*cursorCol > len) {

                *cursorCol = len;
            }
        }
    }

    // KIRI
    else if (ch == 75) {

        // masih dalam baris
        if (*cursorCol > 0) {

            (*cursorCol)--;
        }

        // pindah ke akhir baris atas
        else if ((*currentLine)->prev != NULL) {

            *currentLine = (*currentLine)->prev;

            (*cursorRow)--;

            *cursorCol = strlen((*currentLine)->line);
        }
    }

    // KANAN
    else if (ch == 77) {

        int len = strlen((*currentLine)->line);

        // masih dalam baris
        if (*cursorCol < len) {

            (*cursorCol)++;
        }

        // pindah ke awal baris bawah
        else if ((*currentLine)->next != NULL) {

            *currentLine = (*currentLine)->next;

            (*cursorRow)++;

            *cursorCol = 0;
        }
    }
}