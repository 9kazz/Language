#ifndef REVERSE_H
#define REVERSE_H

#include <stdio.h>
#include "types.h"

TreeNode_t* Reverse_Translate (TreeNode_t* node, int* offset, Bools if_func_init);

TreeNode_t* Translate_Oper    (TreeNode_t* node, int* offset);
TreeNode_t* Translate_Block   (TreeNode_t* node, int* offset);
TreeNode_t* Translate_Args    (TreeNode_t* node, int* offset, Bools if_func_init);
TreeNode_t* Translate_Var_Init(TreeNode_t* node, int* offset, Bools if_func_init);

TreeErr_t   Print_Offset      (int offset);

#endif