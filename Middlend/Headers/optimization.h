#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <stdio.h>

#include "Logfile.h"
#include "types.h"

TreeNode_t* Optimization_Func    (TreeNode_t* node);
TreeNode_t* Const_Folding_OPT    (TreeNode_t* node,  Bools* Tree_was_Changed);
TreeNode_t* Del_Neutral_Elem_OPT (TreeNode_t* node,  Bools* Tree_was_Changed);

double      Calculate_Tree       (TreeNode_t* node, const char* var, double point);
double      Calculate_Oper       (Token_t oper, double num_1, double num_2);

Bools       Compare_Son_with_Num (TreeNode_t* node, NodeSon son, double value, Compare relation);
Bools       Is_type_num          (TreeNode_t* node, NodeSon son);
Compare     Double_compare       (double num_1, double num_2);
Bools       Is_unary_oper        (Token_t oper_code);

#endif