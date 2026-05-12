#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "stack_func.h"
#include "stack_var.h"
#include "tree_utils.h"



int Count_Func_Args(TreeNode_t* cur_node) {
    assert(cur_node);

    int args_count = 1;

    if ( ! LEFT(func_decl_node) ) {
        args_count = 0;
    
    } else {
        TreeNode_t* temp_node_ptr = LEFT(func_decl_node);

        while ( TYPE( temp_node_ptr ) == TYPE_OPER && DATA(temp_node_ptr).oper == _COMMA_ ) {
            args_count++ ;
            temp_node_ptr = LEFT(temp_node_ptr);
        }
    }

    return args_count;
}

int Count_Vars_Init_in_Func(TreeNode_t* func_decl_node) {
    assert(func_decl_node);

    TreeNode_t* cur_node = RIGHT(func_decl_node);

    int vars_count = 0;

    if ( TYPE(cur_node) == _VAR_INIT_ ) {
        vars_count++ ;
        return vars_count;    
    }

    if ( LEFT(cur_node) )
        vars_count += Count_Vars_Init_in_Func( LEFT(cur_node) );

    if ( RIGHT(cur_node) )
        vars_count += Count_Vars_Init_in_Func( RIGHT(cur_node) );

    return vars_count;
}