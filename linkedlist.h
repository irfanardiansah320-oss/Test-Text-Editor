#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#define MAX_COLS 256

// =========================
// STRUCT NODE
// =========================
typedef struct Node {

    char line[MAX_COLS];

    struct Node *next;

} Node;


// =========================
// FUNCTION PROTOTYPE
// =========================

// membuat node baru
Node* createNode(char line[]);

// menambahkan node di akhir linked list
void appendNode(Node **head, char line[]);

// mengambil node berdasarkan index
Node* getNodeAt(Node *head, int index);

// menghitung jumlah node
int getRowCount(Node *head);

// menghapus semua node
void freeList(Node *head);

#endif