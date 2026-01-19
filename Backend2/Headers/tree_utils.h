#ifndef TREE_UTILS_H
#define TREE_UTILS_H

FNameTable_t* Add_Func_to_NameTable(TreeNode_t* func_decl_node, Fstack_str* NameTable);

int Count_Func_Args         (TreeNode_t* func_decl_node);
int Count_Vars_Init_in_Func (TreeNode_t* func_decl_node);

StackErr_t Print_NameTable  (Fstack_str* NameTable);
#endif