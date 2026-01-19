#ifndef ASM_TRANSLATE
#define ASM_TRANSLATE

#include <stdio.h>
#include "types.h"
#include "stack_func.h"

void Asm_Main_Function(TreeNode_t* tree_root);

TreeNode_t* Asm_Translate(TreeNode_t* cur_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable);
Fstack_str* Create_func_NameTable(void);
FNameTable_t* Add_Func_to_NameTable(TreeNode_t* func_decl_node, Fstack_str* func_NameTable);
Vstack_str* Add_Var_to_NameTable(TreeNode_t* var_decl_node, Vstack_str* var_NameTable);

StackErr_t Print_NameTable(Fstack_str* FNameTable);

#endif