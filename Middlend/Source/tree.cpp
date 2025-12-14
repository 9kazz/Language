#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "verify.h"
#include "dump.h"
#include "types.h"

Tree_t* Tree_Ctor(TreeNode_t* root) {
    assert(root);

    SAFE_CALLOC(tree, 1, Tree_t)

    ROOT(tree) = root;
    DISK(tree) = NULL;

    ONDEBUG( Node_Verify(ROOT(tree)); )
    
    ONDEBUG( DUMP_AFTER(tree) )

    return tree;
}

TreeErr_t Tree_Dtor(Tree_t* tree) {
    assert(tree);

    // ONDEBUG( DUMP_BEFORE(tree) ) 

    if ( ! tree) {
        fprintf(stderr, "Node_Dtor: NULL-pointer to TreeNode_t\n");
        return EC_NULL_POINTER;
    }

    Node_Verify(ROOT(tree));
    Node_Dtor(ROOT(tree));

    if ( DISK(tree) ) {
        free(DISK_BUF(tree));
        free(DISK(tree));
    }

    free(tree);

    return END_WITH_SUC;
}

TreeNode_t* Node_Ctor(TreeElem_t data, Data_Type type) {

    SAFE_CALLOC(node, 1, TreeNode_t)

    TYPE(node)   = type;
    DATA(node)   = data;

    LEFT(node)   = NULL;
    RIGHT(node)  = NULL;
    PARENT(node) = NULL;

    ONDEBUG( Node_Verify(node); )
    return node;
}

TreeErr_t Node_Dtor(TreeNode_t* node) {
    assert(node);
    ONDEBUG( Node_Verify(node); )

    if ( ! node) {
        fprintf(stderr, "Node_Dtor: NULL-pointer to TreeNode_t\n");
        return EC_NULL_POINTER;
    }

    if (LEFT(node))
        Node_Dtor(LEFT(node));
    
    if (RIGHT(node))
        Node_Dtor(RIGHT(node));

    free(node);

    return END_WITH_SUC;
}

int Is_Leaf_Node(TreeNode_t* node) {
    assert(node);
    ONDEBUG( Node_Verify(node); )

    if (LEFT(node)  == NULL && 
        RIGHT(node) == NULL )
    return IS_LEAF;

    return NOT_LEAF;
}