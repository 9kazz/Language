#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "lexical.h"
#include "tree.h"
#include "dump.h"
#include "utils.h"

FILE* TreeLog            = fopen("../UtilFiles/FrontLog.htm", "w");
FILE* TreeDisk_Backend   = fopen("../ProgFiles/Tree.txt", "w");

void Logfiles_close(void) {
    fclose(TreeLog);
    fclose(TreeDisk_Backend);
}

int main() {
    assert(TreeLog);
    assert(TreeDisk_Backend);

    Tree_t* tree = Create_Tree_from_disk("../ProgFiles/Program.txt");
    Graphic_Dump_Tree ( ROOT(tree), "Frontend AST", "../UtilFiles");
    Dump_Node_preorder( ROOT(tree), TreeDisk_Backend);
    Tree_Dtor(tree);

    atexit(Logfiles_close);
    return 0;
}