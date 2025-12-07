#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "lexical.h"
#include "tree.h"

FILE* TreeLog = fopen("Logfile.htm", "w");

void Logfiles_close(void) {
    fclose(TreeLog);
}

int main() {
    assert(TreeLog);

    Tree_t* tree = Create_Tree_from_disk("disk_input.txt");
    Tree_Dtor(tree);

    atexit(Logfiles_close);
    return 0;
}