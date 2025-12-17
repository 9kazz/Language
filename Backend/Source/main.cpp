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

FILE* Logfile  = fopen("Logfile.htm", "w");
// FILE* Asm_file = fopen("dump_files/asm_input.txt", "w");
FILE* Asm_file = fopen("../../Virtual_machine/Assembler/input_files/language.txt", "w");

void Logfile_close(void) {
    fclose(Logfile);
    fclose(Asm_file);
}
 
int main() {
    assert(Logfile);
    assert(Asm_file);

    Tree_t* tree = Create_Tree_from_disk("Tree.txt");
    Graphic_Dump_Node(tree, "Tree created from disk: " __FILE__);

    Stack_str* NameTable = Stack_Ctor(10);

    Asm_Translate( ROOT(tree), NameTable );
    fprintf(Asm_file, "HLT");

    Tree_Dtor(tree);
    Stack_Dtor(NameTable);

    atexit(Logfile_close);
    return 0;
}