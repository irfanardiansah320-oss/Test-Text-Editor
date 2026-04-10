#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_cursor.h"

// ambil dari editor kamu
extern char text[][256];
extern int rowCount;

void findAndReplace() {
    char filename[100];
    char find[100], replace[100];
    char buffer[1000];
    char result[10000] = "";

    // =========================
    // INPUT NAMA FILE
    // =========================
    printf("Masukkan nama file: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File tidak ditemukan!\n");
        return;
    }
    fclose(fp);

    // =========================
    // SET TAMPILAN EDITOR
    // =========================
    strcpy(text[0], "Kata yang ingin dicari: ");
    strcpy(text[1], "Kata pengganti: ");
    rowCount = 2;

    // =========================
    // MASUK EDITOR (CURSOR HIDUP)
    // =========================
    runEditor(filename, 0);

    // =========================
    // AMBIL INPUT DARI EDITOR
    // =========================
    strcpy(find, text[0] + strlen("Kata yang ingin dicari: "));
    strcpy(replace, text[1] + strlen("Kata pengganti: "));

    if (strlen(find) == 0) {
        printf("Input tidak boleh kosong!\n");
        return;
    }

    // =========================
    // PROSES REPLACE
    // =========================
    fp = fopen(filename, "r");

    while (fgets(buffer, sizeof(buffer), fp)) {
        char temp[1000];
        char *pos, *start = buffer;

        while ((pos = strstr(start, find)) != NULL) {
            strncpy(temp, start, pos - start);
            temp[pos - start] = '\0';

            strcat(result, temp);
            strcat(result, replace);

            start = pos + strlen(find);
        }

        strcat(result, start);
    }

    fclose(fp);

    // =========================
    // SIMPAN HASIL
    // =========================
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    fputs(result, fp);
    fclose(fp);

    printf("Berhasil replace kata!\n");

    // =========================
    // BALIK KE EDITOR LAGI (OPSIONAL)
    // =========================
    runEditor(filename, 0);
}