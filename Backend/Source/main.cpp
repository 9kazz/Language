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
#include "asm_translate.h"

FILE* Logfile  = fopen("../UtilFiles/BackLog.htm", "w");
FILE* Asm_file = fopen("../ProgFiles/Program.asm", "w");

void Logfile_close(void) {
    fclose(Logfile);
    fclose(Asm_file);
}
 
int main() {
    assert(Logfile);
    assert(Asm_file);

    Tree_t* tree = Create_Tree_from_disk("../ProgFiles/Tree.txt");
    
    Graphic_Dump_Tree( ROOT(tree), "Backtend AST", "../UtilFiles");

    Asm_Main_Function( ROOT(tree) );

    Tree_Dtor(tree);

    atexit(Logfile_close);
    return 0;
}