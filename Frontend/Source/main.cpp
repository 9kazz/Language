#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "lexical.h"
#include "tree.h"
#include "dump.h"
#include "utils.h"

FILE* TreeLog  = fopen("dump_files/Logfile.htm", "w");
FILE* TreeDisk = fopen("dump_files/Tree.txt", "w");

void Logfiles_close(void) {
    fclose(TreeLog);
    fclose(TreeDisk);
}

int main() {
    assert(TreeLog);
    assert(TreeDisk);

    Tree_t* tree = Create_Tree_from_disk("disk_input.txt");
    Graphic_Dump_Tree( ROOT(tree), "AST");
    Dump_Node_preorder( ROOT(tree), TreeDisk);
    Tree_Dtor(tree);

    atexit(Logfiles_close);
    return 0;
}