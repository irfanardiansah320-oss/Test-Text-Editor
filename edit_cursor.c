#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "edit_cursor.h"

#define MAX_ROWS 10
#define MAX_COLS 5

static char text[MAX_ROWS][MAX_COLS];
static int rowCount = 1;
static int cursorRow = 0, cursorCol = 0;
static HANDLE hConsole;

// Memuat file ke buffer (jika isNew == 0)
static void loadFile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    rowCount = 0;
    while (rowCount < MAX_ROWS && fgets(text[rowCount], MAX_COLS, f)) {
        // hilangkan newline di akhir (karena editor tidak pakai newline internal)
        text[rowCount][strcspn(text[rowCount], "\n")] = '\0';
        rowCount++;
    }
    fclose(f);
    if (rowCount == 0) rowCount = 1;
}

// Menyimpan buffer ke file
static void saveFile(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    for (int i = 0; i < rowCount; i++) {
        fprintf(f, "%s", text[i]);
        if (i < rowCount - 1) fprintf(f, "\n");
    }
    fclose(f);
}

// Render layar dan posisi kursor
static void render() {
    system("cls");
    for (int i = 0; i < rowCount; i++) {
        printf("%s", text[i]);
        if (i < rowCount - 1) printf("\n");
    }
    COORD pos = {cursorCol, cursorRow};
    SetConsoleCursorPosition(hConsole, pos);
}

// Fungsi utama editor
void runEditor(const char *filename, int isNew) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Inisialisasi buffer
    if (!isNew) {
        loadFile(filename);
    } else {
        // file baru: kosongkan semua baris
        for (int i = 0; i < MAX_ROWS; i++) text[i][0] = '\0';
        rowCount = 1;
    }
    cursorRow = 0;
    cursorCol = 0;
    
    int ch;
    while (1) {
        render();
        ch = _getch();
        
        if (ch == 27) { // ESC -> simpan dan keluar
            saveFile(filename);
            break;
        }
        else if (ch == 224) { // tombol panah
            ch = _getch();
            if (ch == 72 && cursorRow > 0) cursorRow--;          // up
            else if (ch == 80 && cursorRow < rowCount-1) cursorRow++; // down
            else if (ch == 75 && cursorCol > 0) cursorCol--;     // left
            else if (ch == 77 && cursorCol < (int)strlen(text[cursorRow])) cursorCol++; // right
        }
        else if (ch == 8) { // backspace
            if (cursorCol > 0) {
                // hapus karakter di kiri kursor
                int len = strlen(text[cursorRow]);
                for (int i = cursorCol; i <= len; i++)
                    text[cursorRow][i-1] = text[cursorRow][i];
                cursorCol--;
            }
            else if (cursorRow > 0) {
                // gabung dengan baris sebelumnya
                int prevLen = strlen(text[cursorRow-1]);
                strcat(text[cursorRow-1], text[cursorRow]);
                // hapus baris saat ini
                for (int i = cursorRow; i < rowCount-1; i++)
                    strcpy(text[i], text[i+1]);
                rowCount--;
                cursorRow--;
                cursorCol = prevLen;
            }
        }
        else if (ch == 13) { // Enter
            // sisipkan baris baru
            for (int i = rowCount; i > cursorRow+1; i--)
                strcpy(text[i], text[i-1]);
            strcpy(text[cursorRow+1], &text[cursorRow][cursorCol]);
            text[cursorRow][cursorCol] = '\0';
            rowCount++;
            cursorRow++;
            cursorCol = 0;
        }
        else if (ch >= 32 && ch <= 126) { // karakter printable
            // sisipkan karakter
            int len = strlen(text[cursorRow]);
            for (int i = len; i >= cursorCol; i--)
                text[cursorRow][i+1] = text[cursorRow][i];
            text[cursorRow][cursorCol] = ch;
            cursorCol++;
        }
        // tombol delete (biasanya 83 pada panah, tapi kita tambah)
        else if (ch == 83) { // Delete (bisa juga dari _getch setelah 224, sudah ditangani? Tidak, kita tangani di sini)
            if (cursorCol < (int)strlen(text[cursorRow])) {
                int len = strlen(text[cursorRow]);
                for (int i = cursorCol+1; i <= len; i++)
                    text[cursorRow][i-1] = text[cursorRow][i];
            }
            else if (cursorRow < rowCount-1) {
                strcat(text[cursorRow], text[cursorRow+1]);
                for (int i = cursorRow+1; i < rowCount-1; i++)
                    strcpy(text[i], text[i+1]);
                rowCount--;
            }
        }
    }
}