#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "edit_cursor.h"
#include "zidan.h"
#include "irfan1.h"

 Node *head = NULL;
int cursorRow = 0;
int cursorCol = 0;
HANDLE hConsole;

 void loadFile(const char *filename) { //Fungsi dibuat oleh Rayhan
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
 void saveFile(const char *filename) { //Fungsi dibuat oleh Irfan
    FILE *f = fopen(filename, "w");
    if (!f) return;

    for (int i = 0; i < rowCount; i++) {
        fprintf(f, "%s", text[i]);
        if (i < rowCount - 1) fprintf(f, "\n");
    }
    fclose(f);
}

 void render() {

    COORD topLeft = {0, 0};

    SetConsoleCursorPosition(hConsole, topLeft);

    Node *curr = head;

    while (curr != NULL) {

        printf("%s", curr->line);

        printf("\x1b[K");

        if (curr->next != NULL) {

            printf("\n");
        }

        curr = curr->next;
    }

    printf("\x1b[J");

    COORD pos = {cursorCol, cursorRow};

    SetConsoleCursorPosition(hConsole, pos);
}

// Fungsi utama editor
void runEditor(const char *filename, int isNew) { //Fungsi dibuat oleh Rayhan
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
        
        if (ch == 27) { // ESC untuk keluar dan simpan, dibuat oleh Rayhan
            saveFile(filename);
            break;
        }

        if (ch == 224) { // Memanggil fungsi cursor movement dari zidan.c
            ch = _getch();
            void handleCursorMovement(int ch, int *cursorRow, int *cursorCol, Node *head);
        }

        else {
            handleTextEditing(ch, text, &cursorRow, &cursorCol, &rowCount); //Memanggil fungsi handleTextEditing dari irfan1.c
        }
    }
}