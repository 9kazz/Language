#ifndef NAMETABLE_H
#define NAMETABLE_H

#include <stdio.h>
#include "types.h"
#include "stack_func.h"

const int INIT_FUNC_STK_CAPASITY = 8;
const int INIT_MAIN_VARS_COUNT   = 8;

Fstack_str* Create_func_NameTable(void);
Vstack_str* Add_Func_to_NameTable(TreeNode_t* func_decl_node, Fstack_str* func_NameTable);
Vstack_str* Add_Var_to_NameTable(TreeNode_t* var_decl_node, Vstack_str* var_NameTable);

size_t Find_Var_in_NameTable(Vstack_str* var_NameTable, char* name);
size_t Find_Func_in_NameTable(Fstack_str* func_NameTable, char* name);

StackErr_t Print_NameTable(Fstack_str* FNameTable);

#endif