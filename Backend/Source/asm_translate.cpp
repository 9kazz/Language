#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "stack.h"

TreeErr_t Asm_Translate(TreeNode_t* node, Stack_str* NameTable) {
    assert(node);
    assert(NameTable);

    switch( TYPE(node) )
    {
        case TYPE_VAR_INIT:
            

        case TYPE_FUNC_INIT:

        // case TYPE_OPER:
        // case TYPE_NUM:
        // case TYPE_VAR:
        // case TYPE_FUNC:

    }
}