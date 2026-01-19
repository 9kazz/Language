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
#include "asm_translate.h"

void Asm_Main_Function(TreeNode_t* tree_root) {
    assert(tree_root);

    Fstack_str* func_NameTable = Create_func_NameTable();

    Asm_Translate(tree_root, func_NameTable, STK_DATA(func_NameTable)[0].vars_stk);

    Print_NameTable(func_NameTable);

    func_Stack_Dtor(func_NameTable);
}

TreeNode_t* Asm_Translate(TreeNode_t* cur_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable) {
    assert(func_NameTable);
    assert(var_NameTable);

    if ( ! cur_node )
        return NULL;

    switch( TYPE(cur_node) )
    {
    case TYPE_OPER:
        Asm_Translate( LEFT(cur_node), func_NameTable, var_NameTable);
        Asm_Translate( RIGHT(cur_node), func_NameTable, var_NameTable);
        break;

    case TYPE_NUM:
        break;
        
    case TYPE_VAR:
        break;

    case TYPE_FUNC:
        break;

    case TYPE_VAR_INIT:
        Add_Var_to_NameTable(cur_node, var_NameTable);
        break;

    case TYPE_FUNC_INIT:
        Add_Func_to_NameTable(cur_node, func_NameTable);
        break;
        
    default:
        fprintf(stderr, "%s at %s:%d: unknown type\n", __func__, __FILE__, __LINE__);
        break;
    }

    return NULL;
}

Fstack_str* Create_func_NameTable(void)
{
    const int INIT_FUNC_STK_CAPASITY = 8;
    const int INIT_MAIN_VARS_COUNT   = 8;

    Fstack_str* func_NameTable = func_Stack_Ctor(INIT_FUNC_STK_CAPASITY);

    FNameTable_t func_info = 
    {
        .name     = "main",
        .vars_stk = var_Stack_Ctor(INIT_MAIN_VARS_COUNT)
    };

    func_Stack_Push(func_NameTable, func_info);

    return func_NameTable;
}

FNameTable_t* Add_Func_to_NameTable(TreeNode_t* func_decl_node, Fstack_str* func_NameTable) {
    assert(func_decl_node);
    assert(func_NameTable);

    const int   START_ARGS_COUNT = 3;
    Vstack_str* loc_var_Name_Table = var_Stack_Ctor(START_ARGS_COUNT);

    Asm_Translate( LEFT(func_decl_node), func_NameTable, loc_var_Name_Table);
    Asm_Translate( RIGHT(func_decl_node), func_NameTable, loc_var_Name_Table);

    FNameTable_t func_info = 
    {
        .name     = DATA(func_decl_node).identifier,
        .vars_stk = loc_var_Name_Table
    };

    func_Stack_Push(func_NameTable, func_info);
    
    return STK_DATA(func_NameTable) + STK_SIZE(func_NameTable) - 1;
}

Vstack_str* Add_Var_to_NameTable(TreeNode_t* var_decl_node, Vstack_str* var_NameTable) {
    assert(var_decl_node);
    assert(var_NameTable);

    var_Stack_Push(var_NameTable, DATA(var_decl_node).identifier);
    
    return var_NameTable;
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