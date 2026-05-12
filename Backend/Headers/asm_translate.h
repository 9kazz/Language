#ifndef ASM_TRANSLATE
#define ASM_TRANSLATE

#include <stdio.h>
#include "types.h"
#include "stack_func.h"

void Asm_Main_Function(TreeNode_t* tree_root);

TreeNode_t* Asm_Translate(TreeNode_t* cur_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable);

TreeNode_t* Asm_Translate_Oper(TreeNode_t* cur_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable);
TreeNode_t* Asm_Translate_Func_Init(TreeNode_t* func_decl_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable);
TreeNode_t* Asm_Translate_Func_Call(TreeNode_t* func_call_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable);
TreeNode_t* Asm_Translate_Var(TreeNode_t* var_node, Vstack_str* var_NameTable);
TreeNode_t* Asm_Translate_Num(TreeNode_t* num_node);

size_t Relocate_RAM_ptr(direction reloc_dir, TreeNode_t* node, Fstack_str* func_NameTable, Vstack_str* var_NameTable);
TreeNode_t* Init_Func_Params(TreeNode_t* cur_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable);


#endif