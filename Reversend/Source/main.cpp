#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "dump.h"
#include "read_tree.h" 
#include "types.h"
#include "reverse.h"

FILE* Logfile     = fopen("../UtilFiles/ReversLog.htm", "w");
FILE* output_file = fopen("../ProgFiles/Reversed.txt", "w");

void Logfile_close(void) {
    fclose(Logfile);
    fclose(output_file);
}
 
int main() {
    assert(Logfile);
    assert(output_file);

    Tree_t* tree = Create_Tree_from_disk("../ProgFiles/Tree.txt");

    Graphic_Dump_Tree( ROOT(tree), "Reversend AST", "../UtilFiles");

    int offset = 0;
    Reverse_Translate( ROOT(tree), &offset, FALSE);

    Tree_Dtor(tree);

    atexit(Logfile_close);
    return 0;
}