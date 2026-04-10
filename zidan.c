#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_cursor.h"   // tambahkan
#include "zidan.h"

void findAndReplace() {
    char filename[100];
    char find[100], replace[100];
    char buffer[1000];
    char result[10000] = "";

    printf("Masukkan nama file: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File tidak ditemukan!\n");
        return;
    }

    strcpy(text[0], "Kata yang ingin dicari: ");
    strcpy(text[1], "Kata pengganti: ");
    rowCount = 2;

    runEditor(filename, 0);

    strcpy(find, text[0] + strlen("Kata yang ingin dicari: "));
    strcpy(replace, text[1] + strlen("Kata pengganti: "));

    if (strlen(find) == 0) {
        printf("Input tidak boleh kosong!\n");
        return;
    }

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

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    fputs(result, fp);
    fclose(fp);

    printf("Berhasil replace kata!\n");
    // CLEAR BUFFER
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    runEditor(filename, 0);
}

void handleCursorMovement(int ch, int *cursorRow, int *cursorCol, int rowCount, char text[][256])
{
    if (ch == 72 && *cursorRow > 0) {
        (*cursorRow)--; // up
    }
    else if (ch == 80 && *cursorRow < rowCount - 1) {
        (*cursorRow)++; // down
    }
    else if (ch == 75 && *cursorCol > 0) {
        (*cursorCol)--; // left
    }
    else if (ch == 77 && *cursorCol < (int)strlen(text[*cursorRow])) {
        (*cursorCol)++; // right
    }
}