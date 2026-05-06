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
void handleCursorMovement(int ch, int *cursorRow, int *cursorCol, Node *head)
{
    Node *curr = getNodeAt(head, *cursorRow);

    // ATAS
    if (ch == 72 && *cursorRow > 0) {

        (*cursorRow)--;

        curr = getNodeAt(head, *cursorRow);

        int maxcol = strlen(curr->line);

        if (*cursorCol > maxcol)
            *cursorCol = maxcol;
    }

    // BAWAH
    else if (ch == 80) {

        Node *next = getNodeAt(head, *cursorRow + 1);

        if (next != NULL) {

            (*cursorRow)++;

            int maxcol = strlen(next->line);

            if (*cursorCol > maxcol)
                *cursorCol = maxcol;
        }
    }

    // KIRI
    else if (ch == 75) {

        if (*cursorCol > 0) {
            (*cursorCol)--;
        }
        else if (*cursorRow > 0) {

            (*cursorRow)--;

            curr = getNodeAt(head, *cursorRow);

            *cursorCol = strlen(curr->line);
        }
    }

    // KANAN
    else if (ch == 77) {

        int len = strlen(curr->line);

        if (*cursorCol < len) {
            (*cursorCol)++;
        }
        else {

            Node *next = getNodeAt(head, *cursorRow + 1);

            if (next != NULL) {

                (*cursorRow)++;

                *cursorCol = 0;
            }
        }
    }
}