#ifndef ZIDAN_H
#define ZIDAN_H
#define MAX_COLS 80

void findAndReplace();
void handleCursorMovement(int ch, int *cursorRow, int *cursorCol, int rowCount, char text[][MAX_COLS]);


#endif