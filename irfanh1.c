#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // <-- tambahkan ini
#include "irfan1.h"
#include "edit_cursor.h"   // tambahkan header editor kursor

void createNewFile() {
    char filename[100];
    printf("Masukkan Nama File : ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    
    // Langsung jalankan editor dengan file kosong
    runEditor(filename, 1);   // 1 = file baru
    printf("File berhasil disimpan.\n");
}

void exitEditor() {
    printf("Terimakasih!\n");
    exit(0);
}