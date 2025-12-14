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

    Tree_t* tree_from_disk = Create_Tree_from_disk("Tree.txt");

    Graphic_Dump_Node(tree_from_disk, "Tree create from disk: " __FILE__);
    Dump_tree_into_file( tree_from_disk->root, "dump_files/disk_output.txt");

    Tree_Dtor(tree_from_disk);
    
    atexit(Logfile_close);
    return 0;
}