#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "dump.h"
#include "verify.h"
#include "types.h"

#define PARENT(node) node->parent
#define LEFT(node)   node->left
#define RIGHT(node)  node->right

#define VERIFY(node)                                                                                                                                \
    {                                                                                                                                               \
    static char verify_info_str[MAX_DUMP_INFO_LEN] = {0};                                                                                           \
    snprintf(verify_info_str, MAX_DUMP_INFO_LEN, "Before <span style = \"color: blue;\"> %s </span> in %s:%d", __func__, __FILE__, __LINE__);       \
                                                                                                                                                    \
    TreeErr_t error = Node_Verify(node);                                                                                                            \
                                                                                                                                                    \
    if (error >= EC_MIN_CRITICAL_ERROR)                                                                                                             \
        return error;                                                                                                                               \
    }


TreeErr_t Node_Verify (TreeNode_t* node) {

    TreeErr_t Error_code = 0;

    #define IF_CRITICAL_ERR                                                                                                                    \
        if (Error_code >= EC_MIN_CRITICAL_ERROR) {                                                                                             \
            fprintf(Logfile, "<h3 style = \"color: red;\"> List_Verify: verification ended with critical error: %d\n </h3>", Error_code);      \
            return Error_code;                                                                                                                 \
        }


    if_null_pointer(node, &Error_code); IF_CRITICAL_ERR
    check_ancestor(node, &Error_code);  IF_CRITICAL_ERR

    #undef IF_CRITICAL_ERR

    if (Error_code != 0)
        fprintf(Logfile, "<h3 style = \"color: orange;\"> List_Verify: verification ended with warnings: %d\n </h3>", Error_code);
    
    return Error_code;
}

TreeErr_t if_null_pointer(void* pointer, TreeErr_t* Error_code) {

    if (pointer == NULL) {                                                                                                                                                                            
        PRINT_ERROR(EC_NULL_POINTER, "NULL pointer")
    }

    return *Error_code;
}

TreeErr_t check_ancestor(TreeNode_t* node, TreeErr_t* Error_code) {

    if ( PARENT(node) ) 
    {
        if ( LEFT(PARENT(node))  != node &&
             RIGHT(PARENT(node)) != node )
        {
            PRINT_ERROR(EC_PARENT_ERR, "Damaged connection with parent\n");
        }
    }

    return *Error_code;
}