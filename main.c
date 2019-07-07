#include <stdio.h>
#include <string.h>
#include "fs.h"


void fs(void){
    FTree *ftree = ftree_init();
    File d1;
    strcpy(d1.file_name, "/");
    d1.is_directory = 1;
    strcpy(d1.file_content, "root");

    File d2;
    strcpy(d2.file_name, "bin");
    d2.is_directory = 1;
    strcpy(d2.file_content, "bin");

    File d3;
    strcpy(d3.file_name, "etc");
    d3.is_directory = 1;
    strcpy(d3.file_content, "etc");

    File d4;
    strcpy(d4.file_name, "man");
    d4.is_directory = 0;
    strcpy(d4.file_content, "man");

    File d5;
    strcpy(d5.file_name, "mysql");
    d5.is_directory = 0;
    strcpy(d5.file_content, "mysql");

    File d6;
    strcpy(d6.file_name, "mongodb");
    d6.is_directory = 0;
    strcpy(d6.file_content, "mongodb");

    ftree_add(ftree, "/", &d1);
    ftree_add(ftree, "/", &d2);
    ftree_add(ftree, "/", &d3);
    ftree_add(ftree, "/bin", &d4);
    ftree_add(ftree, "/usr/db", &d5);
    ftree_add(ftree, "/usr/db", &d6);
    ftree_traverse(ftree);
    printf("%d\n", ftree->size);
    if(ftree_is_exist(ftree, "/bin/man"))
        printf("/bin/man exists.\n");
    if(ftree_is_exist(ftree, "/bin/mount"))
        printf("/bin/mount exists.\n");
    else
        printf("/bin/mount does not exist.\n");
    ftree_clear(ftree);
}

int main() {
    fs();
    return 0;
}