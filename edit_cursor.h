#ifndef EDIT_CURSOR_H
#define EDIT_CURSOR_H

// Menjalankan editor teks dengan kursor.
// Parameter: filename - nama file yang akan diedit.
//            isNew - 1 jika file baru (kosong), 0 jika file sudah ada (akan dimuat).
void runEditor(const char *filename, int isNew);


#endif