#ifndef TREE_H
#define TREE_H 

#include <stdio.h>



Tree_t*     Tree_Ctor(TreeNode_t* root);
TreeErr_t   Tree_Dtor(Tree_t* tree);

TreeNode_t* Node_Ctor(Data_Type type, TreeElem_t data, TreeNode_t* left_son, TreeNode_t* right_son);
TreeNode_t* Node_Ctor_empty(void);
TreeErr_t   Node_Dtor(TreeNode_t* node);
TreeNode_t* Node_Copy(TreeNode_t* node);

int         Is_Leaf_Node(TreeNode_t* node);

#endif