#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "tree.h"
#include "utils.h"
#include "verify.h"
#include "stack.h"

Tree_t* Tree_Ctor(TreeNode_t* root) {
    
    SAFE_CALLOC(tree, 1, Tree_t)

    ROOT(tree)     = root;
    TREE_TKN(tree) = NULL;
    
    // ONDEBUG( DUMP_AFTER(tree) )

    return tree;
}

TreeErr_t Tree_Dtor(Tree_t* tree) {
    assert(tree);

    // ONDEBUG( DUMP_BEFORE(tree) ) 

    if ( ! tree) {
        fprintf(stderr, "Node_Dtor: NULL-pointer to TreeNode_t\n");
        return EC_NULL_POINTER;
    }

    Node_Dtor(ROOT(tree));
    Stack_Dtor(TREE_TKN(tree));
    
    free(tree);

    return END_WITH_SUC;
}

TreeNode_t* Node_Ctor(TreeElem_t data, TreeNode_t* left_son, TreeNode_t* right_son) {
    
    SAFE_CALLOC(node, 1, TreeNode_t)

    NODE_TKN(node) = data;

    LEFT(node)   = left_son;
    RIGHT(node)  = right_son;
    PARENT(node) = NULL;

    if ( LEFT(node) )
        PARENT(left_son)  = node;

    if ( RIGHT(node) )
        PARENT(right_son) = node;

    return node;
}

TreeErr_t Node_Dtor(TreeNode_t* node) {
    assert(node);

    if ( ! node)
        return END_WITH_SUC;

    if (LEFT(node))
        Node_Dtor(LEFT(node));
    
    if (RIGHT(node))
        Node_Dtor(RIGHT(node));

    free(node);

    return END_WITH_SUC;
}

TreeNode_t* Node_Copy(TreeNode_t* node) {

    if ( ! node)
        return NULL; 

    TreeNode_t* new_node = Node_Ctor( NODE_TKN(node), Node_Copy(LEFT(node)), Node_Copy(RIGHT(node)) );

    if ( LEFT(new_node) )
        PARENT( LEFT(new_node) )  = new_node;

    if ( RIGHT(new_node) )
        PARENT( RIGHT(new_node) ) = new_node;

    return new_node;
}

int Is_Leaf_Node(TreeNode_t* node) {
    assert(node);

    if (LEFT(node)  == NULL && 
        RIGHT(node) == NULL )
    return IS_LEAF;

    return NOT_LEAF;
}