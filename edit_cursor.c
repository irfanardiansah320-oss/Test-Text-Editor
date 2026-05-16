#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "edit_cursor.h"
#include "zidan.h"      // untuk handleCursorMovement
#include "irfan1.h"     // untuk handleTextEditing (akan kita ubah juga nanti)

 // Ubah bagian ini
Node *editorHead = NULL; // Nama harus sesuai dengan yang kamu pakai di bawah
Node *currentLine = NULL;  
int cursorRow = 0;
int cursorCol = 0;
int rowCount = 0;       // Tambahkan ini karena sebelumnya belum ada
HANDLE hConsole;

// Memuat file ke dalam linked list
static void loadFileToLinkedList(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;
    char buffer[MAX_COLS];
    while (fgets(buffer, MAX_COLS, f)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        appendNode(&editorHead, buffer);
    }
    fclose(f);
    rowCount = getRowCount(editorHead);
    if (rowCount == 0) {
        // file kosong: buat satu baris kosong
        appendNode(&editorHead, "");
        rowCount = 1;
    }
}

// Menyimpan linked list ke file
static void saveLinkedListToFile(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    Node *curr = editorHead;
    while (curr != NULL) {
        fprintf(f, "%s", curr->line);
        if (curr->next != NULL) fprintf(f, "\n");
        curr = curr->next;
    }
    fclose(f);
}

// Render seluruh layar dari linked list
static void render() {
    COORD topLeft = {0, 0};

    SetConsoleCursorPosition(hConsole, topLeft);
    Node *curr = editorHead;
    int row = 0;
    while (curr != NULL) {
        printf("%s", curr->line);
        printf("\x1b[K");   // hapus sampai akhir baris
        if (curr->next != NULL) printf("\n");
        curr = curr->next;
        row++;
    }
    // Jika jumlah baris berkurang, bersihkan sisa layar
    printf("\x1b[J");
    // Posisikan kursor
    COORD pos = {cursorCol, cursorRow};

    SetConsoleCursorPosition(hConsole, pos);
}

// Menyisipkan baris baru pada posisi tertentu (0-based)
static void insertRowAt(int index, const char *content) {
    if (index < 0) index = 0;
    if (index > rowCount) index = rowCount;
    Node *newNode = createNode(content);
    if (index == 0) {
        newNode->next = editorHead;
        editorHead = newNode;
    } else {
        Node *prev = getNodeAt(editorHead, index - 1);
        newNode->next = prev->next;
        prev->next = newNode;
    }
    rowCount++;
}

// Menghapus baris pada indeks tertentu
static void deleteRowAt(int index) {
    if (index < 0 || index >= rowCount) return;
    Node *temp;
    if (index == 0) {
        temp = editorHead;
        editorHead = editorHead->next;
    } else {
        Node *prev = getNodeAt(editorHead, index - 1);
        temp = prev->next;
        prev->next = temp->next;
    }
    free(temp);
    rowCount--;
}

// Mendapatkan pointer ke baris (Node) pada indeks tertentu
static Node* getRowNode(int index) {
    return getNodeAt(editorHead, index);
}

// Fungsi utama editor (menggantikan yang lama)
void runEditor(Node **head, const char *filename, int isNew) {
    // Gunakan linked list dari parameter
    editorHead = *head;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (!isNew && editorHead == NULL) {
        loadFileToLinkedList(filename);
    } else if (isNew && editorHead == NULL) {
        // file baru: satu baris kosong
        appendNode(&editorHead, "");
    }
    rowCount = getRowCount(editorHead);
    cursorRow = 0;
    cursorCol = 0;
    
    int ch;
    while (1) {
        render();
        ch = _getch();
        
        if (ch == 27) { // ESC -> simpan dan keluar
            saveLinkedListToFile(filename);
            break;
        }
        
        if (ch == 224) { // tombol panah
            ch = _getch();
            // Panggil fungsi handleCursorMovement yang sudah ada di zidan.c
            // Fungsi tersebut perlu menerima Node* head, cursorRow, cursorCol
            handleCursorMovement(ch, &cursorRow, &cursorCol, editorHead);
            // setelah kursor pindah, pastikan cursorCol tidak melebihi panjang baris
            Node *currRow = getRowNode(cursorRow);
            if (currRow && cursorCol > (int)strlen(currRow->line))
                cursorCol = strlen(currRow->line);
        } else {
            // Edit teks: kita perlu mengirim editorHead, baris, kolom, rowCount
            // Namun handleTextEditing di irfan1.c masih menggunakan array statis.
            // Kita harus mengubah irfan1.c juga (lihat penjelasan di bawah)
            // Untuk sementara, kita buat fungsi internal di sini.
            // Karena akan mengubah irfan1.c, kita panggil versi baru.
            handleTextEditingLinkedList(ch, &editorHead, &cursorRow, &cursorCol, &rowCount);
        }
    }
    
    // Kembalikan head yang sudah dimodifikasi ke pemanggil
    *head = editorHead;
}