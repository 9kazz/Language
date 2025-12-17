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

FILE* Logfile = fopen("Logfile.htm", "w");

void Logfile_close(void) {
    fclose(Logfile);
}
 
int main() {
    assert(Logfile);

    Tree_t* tree = Create_Tree_from_disk("Tree.txt");
    Graphic_Dump_Node(tree, "Tree created from disk: " __FILE__);

    Tree_Dtor(tree);
    
    atexit(Logfile_close);
    return 0;
}