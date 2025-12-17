#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <stdio.h>

TreeNode_t* Get_Grammar     (Token_str** token); 
TreeNode_t* Get_Statement   (Token_str** token);
TreeNode_t* Get_Operator    (Token_str** token);
TreeNode_t* Get_Block       (Token_str** token);
TreeNode_t* Get_Func_Init   (Token_str** token);
TreeNode_t* Get_Var_Init    (Token_str** token);
TreeNode_t* Get_Return_oper (Token_str** token);
TreeNode_t* Get_While_oper  (Token_str** token);
TreeNode_t* Get_If_oper     (Token_str** token);
TreeNode_t* Get_Else_oper   (Token_str** token);
TreeNode_t* Get_Assignment  (Token_str** token);
TreeNode_t* Get_Logical     (Token_str** token);
TreeNode_t* Get_Expression  (Token_str** token);
TreeNode_t* Get_Term        (Token_str** token);
TreeNode_t* Get_Power       (Token_str** token); 
TreeNode_t* Get_Primary     (Token_str** token);
TreeNode_t* Get_Unary       (Token_str** token);
TreeNode_t* Get_Number      (Token_str** token);
TreeNode_t* Get_Variable    (Token_str** token);
TreeNode_t* Get_Function    (Token_str** token);
TreeNode_t* Get_Identifier  (Token_str** token);

#endif