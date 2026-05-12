#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "utils.h"
#include "tree.h"
#include "stack_func.h"
#include "stack_var.h"
#include "asm_translate.h"
#include "nametable.h"

Fstack_str* Create_func_NameTable(void)
{
    Fstack_str* func_NameTable = func_Stack_Ctor(INIT_FUNC_STK_CAPASITY);

    FNameTable_t func_info = 
    {
        .name     = "main",
        .vars_stk = var_Stack_Ctor(INIT_MAIN_VARS_COUNT)
    };

    func_Stack_Push(func_NameTable, func_info);

    return func_NameTable;
}

Vstack_str* Add_Func_to_NameTable(TreeNode_t* func_decl_node, Fstack_str* func_NameTable) {
    assert(func_decl_node);
    assert(func_NameTable);

    const int   START_ARGS_COUNT = 3;
    Vstack_str* loc_var_Name_Table = var_Stack_Ctor(START_ARGS_COUNT);

    Asm_Translate( LEFT(func_decl_node),  func_NameTable, loc_var_Name_Table);

    FNameTable_t func_info = 
    {
        .name     = DATA(func_decl_node).identifier,
        .vars_stk = loc_var_Name_Table
    };

    func_Stack_Push(func_NameTable, func_info);
    
    return loc_var_Name_Table;
}

Vstack_str* Add_Var_to_NameTable(TreeNode_t* var_decl_node, Vstack_str* var_NameTable) {
    assert(var_decl_node);
    assert(var_NameTable);

    var_Stack_Push(var_NameTable, DATA(var_decl_node).identifier);
    
    return var_NameTable;
}

size_t Find_Var_in_NameTable(Vstack_str* var_NameTable, char* name) {
    assert(var_NameTable);
    assert(name);

    for (size_t idx = 0;  idx < STK_SIZE(var_NameTable);  idx++) 
    {
        if ( strcmp(name, DATA(var_NameTable)[idx]) == 0 )
            return idx;
    }

    return 0;
}

size_t Find_Func_in_NameTable(Fstack_str* func_NameTable, char* name) {
    assert(func_NameTable);
    assert(name);

    for (size_t idx = 0;  idx < STK_SIZE(func_NameTable);  idx++) 
    {
        if ( strcmp(name, DATA(func_NameTable)[idx].name) == 0 )
            return idx;
    }

    return 0;
}

StackErr_t Print_NameTable(Fstack_str* FNameTable) {
    assert(FNameTable);

    fprintf(stdout, "name\tsize\tvars\n");
    fprintf(stdout, "------------------------------------------\n");

    for (size_t func_idx = 0;  func_idx < STK_SIZE(FNameTable);  func_idx++) 
    {
        Fstack_t func_info = DATA(FNameTable)[func_idx];

        fprintf(stdout, "%s\t", func_info.name);
        fprintf(stdout, "%d/%d\t", STK_SIZE(func_info.vars_stk), STK_CAPACITY(func_info.vars_stk));
        
        for (size_t var_idx = 0;  var_idx < STK_SIZE(func_info.vars_stk);  var_idx++) 
        {
            char* var_name = DATA(func_info.vars_stk)[var_idx];

            if ( ! var_name )
                continue;
            
            fprintf(stdout, "%s ",  var_name);
        }

        fprintf(stdout, "\n");
    }

    return STK_NO_ERR;
}