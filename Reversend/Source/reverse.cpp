#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "tokens.h"
#include "tree.h"
#include "stack.h"
#include "reverse.h"

//---------------------------------------------------------------------------------------------------------------------
#define PRINT(format, ...)  fprintf(output_file, format, ##__VA_ARGS__);
#define KEY_WORD(oper)      Token_Info_Arr[oper - 1].key_word

#define CASE_INORDER_OPER(oper)                                 \
    case oper:                                                  \
        Reverse_Translate( LEFT(node), offset, FALSE );         \
        PRINT("%s ", KEY_WORD(oper) );                          \
        return Reverse_Translate( RIGHT(node), offset, FALSE );

#define CASE_UNARY_OPER(oper)                       \
    case oper:                                      \
        PRINT("%s ", KEY_WORD(oper) );              \
        return Translate_Args(node, offset, FALSE);
//---------------------------------------------------------------------------------------------------------------------

TreeNode_t* Reverse_Translate(TreeNode_t* node, int* offset, Bools if_func_init) {
    assert(offset);

    if ( ! node )
        return NULL;

    switch ( TYPE(node) )
    {
    case TYPE_VAR_INIT:
        Translate_Var_Init(node, offset, if_func_init);
        break;
        
    case TYPE_FUNC_INIT:
        PRINT("%s %s ", KEY_WORD(_FUNC_INIT_), DATA(node).identifier);
        Translate_Args(node, offset, TRUE);
        Translate_Block(node, offset);
        break;
        
    case TYPE_OPER:
        Translate_Oper(node, offset);
        break;

    case TYPE_NUM:
        PRINT("%lg ", DATA(node).num);
        break;
        
    case TYPE_VAR:
        PRINT("%s ", DATA(node).identifier);
        break;

    case TYPE_FUNC:
        PRINT("%s ", DATA(node).identifier);
        Translate_Args(node, offset, FALSE);
        break;
    
    default:
        fprintf(stderr, "%s at %s:%d: unknown type\n", __func__, __FILE__, __LINE__);
        break;
    }

    return node;
}

TreeNode_t* Translate_Oper(TreeNode_t* node, int* offset) {
    assert(node);
    assert( TYPE(node) == TYPE_OPER );

    switch ( DATA(node).oper )
    {
    case _END_STATEMENT_:
        Print_Offset(*offset);
        Reverse_Translate( LEFT(node), offset, FALSE );
        PRINT("%s\n", KEY_WORD(_END_STATEMENT_) );
        
        return Reverse_Translate( RIGHT(node), offset, FALSE );
    
    case _IF_:
        PRINT("%s ", KEY_WORD(_IF_) );
        Translate_Args(node, offset, FALSE);
        return Translate_Block(node, offset);

    case _WHILE_:
        PRINT("%s ", KEY_WORD(_WHILE_) );
        Translate_Args(node, offset, FALSE);
        return Translate_Block(node, offset);

    case _RETURN_:
        PRINT("%s ", KEY_WORD(_RETURN_) )
        return Reverse_Translate( LEFT(node), offset, FALSE );

    CASE_UNARY_OPER(_PRINT_);
    CASE_UNARY_OPER(_DRAW_);
    CASE_UNARY_OPER(_MATH_SQRT_);

    CASE_INORDER_OPER(_ASSIGNMENT_);
    CASE_INORDER_OPER(_MATH_ADD_);
    CASE_INORDER_OPER(_MATH_SUB_);
    CASE_INORDER_OPER(_MATH_MUL_);
    CASE_INORDER_OPER(_MATH_DIV_);
    CASE_INORDER_OPER(_LOG_EQUAL_);
    CASE_INORDER_OPER(_LOG_LESS_);
    CASE_INORDER_OPER(_LOG_MORE_);
    CASE_INORDER_OPER(_LOG_NOT_EQUAL_);
    CASE_INORDER_OPER(_LOG_LESS_EQUAL_);
    CASE_INORDER_OPER(_LOG_MORE_EQUAL_);

    default:
        break;
    }
    
    return NULL;
}

TreeNode_t* Translate_Block(TreeNode_t* node, int* offset) {
    assert(node);
    assert(offset);

    (*offset)++ ;

    PRINT("\n");
    Print_Offset(*offset - 1);
    PRINT("%s \n", KEY_WORD(_BEGIN_OPER_));

    Reverse_Translate( RIGHT(node), offset, FALSE );

    Print_Offset(*offset - 1);
    PRINT("%s ", KEY_WORD(_END_OPER_));

    (*offset)-- ;
    return node;
}

TreeNode_t* Translate_Args(TreeNode_t* node, int* offset, Bools if_func_init) {
    assert(node);
    assert(offset);

    PRINT("%s ", KEY_WORD(_OPEN_BRACK_));
    Reverse_Translate( LEFT(node), offset, if_func_init);
    PRINT("%s ", KEY_WORD(_CLOSE_BRACK_));

    return node;
}

TreeNode_t* Translate_Var_Init(TreeNode_t* node, int* offset, Bools if_func_init) {
    assert(node);
    assert(offset);

    if ( ! if_func_init) {
        PRINT("%s %s ", KEY_WORD(_VAR_INIT_), DATA(node).identifier);
        return node;
    }

    PRINT("%s ", DATA(node).identifier);
    return node;
}

TreeErr_t Print_Offset(int offset) {

    for (int counter = 0;  counter < offset;  counter++)
        PRINT("\t");

    return END_WITH_SUC;
}

//---------------------------------------------------------------------------------------------------------------------
#undef PRINT
#undef KEY_WORD
#undef CASE_INORDER_OPER
#undef CASE_UNARY