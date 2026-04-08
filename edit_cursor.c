#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "edit_cursor.h"
#include "zidan.h"
#include "irfan1.h"

#define MAX_ROWS 10
#define MAX_COLS 5

static char text[MAX_ROWS][MAX_COLS];
static int rowCount = 1;
static int cursorRow = 0, cursorCol = 0;
static HANDLE hConsole;

// Load file ke buffer
static void loadFile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;

    rowCount = 0;
    while (rowCount < MAX_ROWS && fgets(text[rowCount], MAX_COLS, f)) {
        text[rowCount][strcspn(text[rowCount], "\n")] = '\0';
        rowCount++;
    }
    fclose(f);

    if (rowCount == 0) rowCount = 1;
}

// Simpan file
static void saveFile(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;

    for (int i = 0; i < rowCount; i++) {
        fprintf(f, "%s", text[i]);
        if (i < rowCount - 1) fprintf(f, "\n");
    }
    fclose(f);
}

// Render tampilan
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

    // Inisialisasi
    if (!isNew) {
        loadFile(filename);
    } else {
        for (int i = 0; i < MAX_ROWS; i++) text[i][0] = '\0';
        rowCount = 1;
    }

    cursorRow = 0;
    cursorCol = 0;

    int ch;

    while (1) {
        render();
        ch = _getch();

        // ESC → simpan & keluar
        if (ch == 27) {
            saveFile(filename);
            break;
        }

        // 🔥 ZIDAN → movement
        if (ch == 224) {
            ch = _getch();
            handleCursorMovement(ch, &cursorRow, &cursorCol, rowCount, text);
        }

        // 🔥 IRFAN → editing
        else {
            handleTextEditing(ch, text, &cursorRow, &cursorCol, &rowCount);
        }
    }
}