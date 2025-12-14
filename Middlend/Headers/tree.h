#ifndef TREE_H
#define TREE_H 

#include "types.h"

Tree_t*     Tree_Ctor(TreeNode_t* root);
TreeErr_t   Tree_Dtor(Tree_t* tree);

TreeNode_t* Node_Ctor(TreeElem_t data, Data_Type type);
TreeErr_t   Node_Dtor(TreeNode_t* node);

int         Is_Leaf_Node(TreeNode_t* node);

#endif