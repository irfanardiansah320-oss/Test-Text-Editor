#ifndef ZIDAN_H
#define ZIDAN_H

#include "edit_cursor.h"
#include "linkedlist.h"



void findAndReplace();
void handleCursorMovement(int ch, int *cursorRow, int *cursorCol, Node *head);

#endif