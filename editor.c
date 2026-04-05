#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "editor.h"

#define MAX 1000

char text[MAX] = "";
int cursor = 0;

// ================= RENDER =================
void renderText(int showCursor) {
    system("cls");

    printf("=== TEXT EDITOR ===\n\n");

    int len = strlen(text);

    for (int i = 0; i < len; i++) {
        if (i == cursor && showCursor) printf("|");
        printf("%c", text[i]);
    }

    if (cursor == len && showCursor) printf("|");

    printf("\n\nESC = keluar\n");
}

// ================= INSERT =================
void insertChar(char c) {
    int len = strlen(text);

    for (int i = len; i >= cursor; i--) {
        text[i + 1] = text[i];
    }

    text[cursor] = c;
    cursor++;
}

// ================= DELETE =================
void deleteChar() {
    if (cursor == 0) return;

    int len = strlen(text);

    for (int i = cursor - 1; i < len; i++) {
        text[i] = text[i + 1];
    }

    cursor--;
}

// ================= MOVE =================
void moveLeft() {
    if (cursor > 0) cursor--;
}

void moveRight() {
    if (cursor < strlen(text)) cursor++;
}

// ================= MAIN EDITOR =================
void editorMode() {
    int showCursor = 1;
    char ch;

    while (1) {

        renderText(showCursor);

        if (_kbhit()) {
            ch = getch();

            if (ch == 27) break; // ESC

            else if (ch == 8) deleteChar(); // backspace

            else if (ch == 224) {
                ch = getch();

                if (ch == 75) moveLeft();
                else if (ch == 77) moveRight();
            }

            else {
                insertChar(ch);
            }
        }

        showCursor = !showCursor;
        Sleep(500);
    }
}