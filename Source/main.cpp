#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "lexical.h"
#include "tree.h"

FILE* Logfile = fopen("Logfile.htm", "w");

void Logfile_close(void) {
    fclose(Logfile);
}

int main() {
    assert(Logfile);

    Tree_t* tree = Create_Tree_from_disk("disk_input.txt");
    Tree_Dtor(tree);

    atexit(Logfile_close);
    return 0;
}