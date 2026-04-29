#ifndef irfan_h1
#define irfan_h1
#define MAX_COLS 80

// Deklarasi fungsi-fungsi editor
void createNewFile();
void exitEditor ();
void handleTextEditing(int ch, char text[][MAX_COLS], int *cursorRow, int *cursorCol, int *rowCount);

#endif