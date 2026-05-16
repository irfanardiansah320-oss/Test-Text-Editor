#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_cursor.h"
#include "zidan.h"

void handleCursorMovement(int ch, Cursor *cur) {
    int currentLen = (int)strlen(cur->current->data);

    if (ch == 72) {
        /* ---- PANAH ATAS ---- */
        /* Syarat: harus ada baris di atas (prev != NULL) */
        if (cur->current->prev != NULL) {
            cur->current = cur->current->prev;  /* ikuti pointer prev */
            cur->cursorRow--;

            /* Clamp kolom: jika baris baru lebih pendek,
             * cursor tidak boleh melewati akhir baris */
            int newLen = (int)strlen(cur->current->data);
            if (cur->cursorCol > newLen) {
                cur->cursorCol = newLen;
            }
        }
    }
    else if (ch == 80) {
        /* ---- PANAH BAWAH ---- */
        /* Syarat: harus ada baris di bawah (next != NULL) */
        if (cur->current->next != NULL) {
            cur->current = cur->current->next;  /* ikuti pointer next */
            cur->cursorRow++;

            /* Clamp kolom */
            int newLen = (int)strlen(cur->current->data);
            if (cur->cursorCol > newLen) {
                cur->cursorCol = newLen;
            }
        }
    }
    else if (ch == 75) {
        /* ---- PANAH KIRI ---- */
        if (cur->cursorCol > 0) {
            /* Masih ada karakter di kiri dalam baris yang sama */
            cur->cursorCol--;
        }
        else if (cur->current->prev != NULL) {
            /* Sudah di kolom 0: pindah ke akhir baris SEBELUMNYA */
            cur->current   = cur->current->prev;
            cur->cursorRow--;
            cur->cursorCol = (int)strlen(cur->current->data);
        }
        /* Jika di baris pertama kolom 0: tidak bergerak */
    }
    else if (ch == 77) {
        /* ---- PANAH KANAN ---- */
        if (cur->cursorCol < currentLen) {
            /* Masih ada karakter di kanan dalam baris yang sama */
            cur->cursorCol++;
        }
        else if (cur->current->next != NULL) {
            /* Sudah di akhir baris: pindah ke awal baris BERIKUTNYA */
            cur->current   = cur->current->next;
            cur->cursorRow++;
            cur->cursorCol = 0;
        }
        /* Jika di baris terakhir kolom akhir: tidak bergerak */
    }
}

void findAndReplace() {
    char filename[100];
    char find[100], replace[100];
    char buffer[1000];
    char result[100000] = "";   /* buffer hasil lebih besar */

    printf("\n\033[1;36m=== FIND AND REPLACE ===\033[0m\n");

    printf("Masukkan nama file  : ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\033[1;31mFile '%s' tidak ditemukan!\033[0m\n", filename);
        return;
    }

    printf("Kata yang dicari    : ");
    fgets(find, sizeof(find), stdin);
    find[strcspn(find, "\n")] = '\0';

    if (strlen(find) == 0) {
        printf("Input tidak boleh kosong!\n");
        fclose(fp);
        return;
    }

    printf("Kata pengganti      : ");
    fgets(replace, sizeof(replace), stdin);
    replace[strcspn(replace, "\n")] = '\0';

    if (strlen(replace) > 50) {
        printf("\n[!] Kata pengganti maksimal 50 karakter!\n");
        fclose(fp);
        return;
    }

    int found = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        char temp[1000];
        char *pos, *start = buffer;

        while ((pos = strstr(start, find)) != NULL) {
            found++;

            /* salin bagian sebelum kata yang ditemukan */
            strncpy(temp, start, pos - start);
            temp[pos - start] = '\0';

            strcat(result, temp);
            strcat(result, replace);

            start = pos + strlen(find);
        }
        /* salin sisa baris setelah penggantian terakhir */
        strcat(result, start);
    }

    fclose(fp);

    if (found == 0) {
        printf("\033[1;33mKata '%s' tidak ditemukan dalam file!\033[0m\n", find);
        return;
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Gagal membuka file untuk ditulis!\n");
        return;
    }

    fputs(result, fp);
    fclose(fp);

    printf("\033[1;32mBerhasil! %d kata '%s' diganti dengan '%s'.\033[0m\n",
           found, find, replace);
}
