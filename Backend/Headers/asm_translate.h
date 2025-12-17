#ifndef ASM_TRANSLATE
#define ASM_TRANSLATE

#include <stdio.h>
#include "types.h"
#include "stack.h"

TreeNode_t* Asm_Translate        (TreeNode_t* node, Stack_str* NameTable);

TreeNode_t* Trans_Oper           (TreeNode_t* node, Stack_str* NameTable);

TreeNode_t* Trans_Func           (TreeNode_t* node, Stack_str* NameTable);
TreeNode_t* Init_func_params     (Stack_str* NameTable, TreeNode_t* node, size_t func_address, size_t* param_counter);

TreeNode_t* Trans_Func_Init      (TreeNode_t* node, Stack_str* NameTable);
Stack_str*  Fill_local_NameTable (TreeNode_t* node, Stack_str* Local_NameTable, NameTable_t* func_info);

#endif