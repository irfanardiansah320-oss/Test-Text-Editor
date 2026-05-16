#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"



// =========================
// CREATE NODE
// =========================
Node* createNode(char line[]) {

    Node *newNode = (Node*)malloc(sizeof(Node));

    strcpy(newNode->line, line);

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}



// =========================
// APPEND NODE
// =========================
void appendNode(Node **head, char line[]) {

    Node *newNode = createNode(line);

    // jika linked list kosong
    if (*head == NULL) {

        *head = newNode;

        return;
    }

    // traversal ke node terakhir
    Node *curr = *head;

    while (curr->next != NULL) {

        curr = curr->next;
    }

    // sambungkan node baru
    curr->next = newNode;
    newNode->prev = curr;
}



// =========================
// GET NODE AT INDEX
// =========================
Node* getNodeAt(Node *head, int index) {

    Node *curr = head;

    int i = 0;

    while (curr != NULL && i < index) {

        curr = curr->next;

        i++;
    }

    return curr;
}



// =========================
// GET ROW COUNT
// =========================
int getRowCount(Node *head) {

    int count = 0;

    Node *curr = head;

    while (curr != NULL) {

        count++;

        curr = curr->next;
    }

    return count;
}



// =========================
// FREE LINKED LIST
// =========================
void freeList(Node *head) {

    Node *temp;

    while (head != NULL) {

        temp = head;

        head = head->next;

        free(temp);
    }
}