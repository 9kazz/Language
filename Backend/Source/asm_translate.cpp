#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "stack.h"
#include "asm_translate.h"

//---------------------------------------------------------------------------------------------------------------------
#define PRINT(format, ...)  fprintf(Asm_file, format, ##__VA_ARGS__);

#define GET_RAM_IDX(name_table, node)  name_table->data[ NameTable_Find_Idfier(name_table, (node)->data.identifier) ].mem_idx

#define CASE_MATH_OPER(oper)                        \
    case _MATH_##oper##_:                           \
        Asm_Translate( LEFT(node),  NameTable );    \
        Asm_Translate( RIGHT(node), NameTable );    \
        PRINT(#oper "\n");                          \
        return node;
//---------------------------------------------------------------------------------------------------------------------

TreeNode_t* Asm_Translate(TreeNode_t* node, Stack_str* NameTable) {
    assert(NameTable);

    if ( ! node )
        return NULL;

        switch( TYPE(node) )
        {
        case TYPE_VAR_INIT:
            NameTable_Push_Var(NameTable, node);
            break;
            
        case TYPE_FUNC_INIT:
            Trans_Func_Init(node, NameTable);
            break;
            
        case TYPE_OPER:
            Trans_Oper(node, NameTable);
            break;

        case TYPE_NUM:
            PRINT("PUSH %lg ; num\n", DATA(node).num);
            break;
            
        case TYPE_VAR:
            PRINT("PUSH %d ; var\n", GET_RAM_IDX(NameTable, node));
            PRINT("POPR RAX\n");
            PRINT("PUSHM [RAX]\n");
            break;

        case TYPE_FUNC:
            Trans_Func(node, NameTable);
            break;
        
        default:
            fprintf(stderr, "%s at %s:%d: unknown type\n", __func__, __FILE__, __LINE__);
            break;
    }

    Print_NameTable(NameTable);

    return NULL;
}

TreeNode_t* Trans_Oper(TreeNode_t* node, Stack_str* NameTable) {
    assert(node);
    assert(NameTable);
    assert( TYPE(node) == TYPE_OPER );

    switch ( DATA(node).oper )
    {
    case _END_STATEMENT_:
               Asm_Translate( LEFT(node),  NameTable );
        return Asm_Translate( RIGHT(node), NameTable );
    
    case _ASSIGNMENT_: 
    {
        size_t var_idx = GET_RAM_IDX( NameTable, LEFT(node) );

        Asm_Translate( RIGHT(node), NameTable );

        PRINT("PUSH %d ; assignment\n", var_idx);
        PRINT("POPR RAX\n");
        PRINT("POPM [RAX]\n");

        return node;
    }

    case _IF_:
        PRINT("PUSH 0 ; if\n");
        Asm_Translate( LEFT(node), NameTable );

        PRINT("JE :if_%d\n", node);

        Asm_Translate( RIGHT(node), NameTable );

        PRINT("if_%d:\n", node);
        PRINT("OUT\n");
        PRINT("OUT\n");

        return node;

    case _WHILE_:
        PRINT("start_while_%d:\n", node);

        PRINT("PUSH 0\n");
        Asm_Translate( LEFT(node), NameTable );
        PRINT("JE :end_while_%d\n", node);

        PRINT("OUT\n");
        PRINT("OUT\n");
        Asm_Translate( RIGHT(node), NameTable );
        PRINT("JMP :start_while_%d\n", node);

        PRINT("end_while_%d:\n", node);
        PRINT("OUT\n");
        PRINT("OUT\n");

        return node;

    // case _LOG_EQUAL_:
    //     Asm_Translate( LEFT(node),  NameTable );
    //     Asm_Translate( RIGHT(node), NameTable );

    //     PRINT("JE :true_equal_%d\n", node);
    //     PRINT("OUT\n");
    //     PRINT("OUT\n");
    //     PRINT("PUSH 0\n");
    //     PRINT("JMP :false_equal_%d\n", node);

    //     PRINT("true_equal_%d:\n", node);
    //     PRINT("OUT\n");
    //     PRINT("OUT\n");
    //     PRINT("PUSH 1\n");
    //     PRINT("false_equal_%d:\n", node);

    CASE_MATH_OPER(ADD)
    CASE_MATH_OPER(SUB)
    CASE_MATH_OPER(MUL)
    CASE_MATH_OPER(DIV)
    CASE_MATH_OPER(SQRT)

    default:
        break;
    }
    
    return NULL;
}

TreeNode_t* Trans_Func(TreeNode_t* node, Stack_str* NameTable) {
    assert(node);
    assert(NameTable);
    assert( TYPE(node) == TYPE_FUNC );

    return node;
}

TreeNode_t* Trans_Func_Init(TreeNode_t* node, Stack_str* NameTable) {
    assert(node);
    assert(NameTable);
    assert( TYPE(node) == TYPE_FUNC_INIT );

    NameTable_t func_info = NameTable_Push_Func(NameTable, node);

    Stack_str* Local_NameTable = Stack_Ctor(func_info.args);

    Fill_local_NameTable (LEFT(node), Local_NameTable, &func_info);

    PRINT("JMP :skip_func_init_%d\n", node);
    PRINT("%s:\n", DATA(node).identifier);
    Asm_Translate( RIGHT(node), Local_NameTable );
    PRINT("RET\n");
    PRINT("skip_func_init_%d:\n", node);

    // Print_NameTable(Local_NameTable);
    Stack_Dtor(Local_NameTable);

    return node;
}

Stack_str* Fill_local_NameTable(TreeNode_t* node, Stack_str* Local_NameTable, NameTable_t* func_info) {
    assert(node);
    assert(Local_NameTable);
    assert(func_info);

    if ( Is_Leaf_Node(node) ) 
    {
        NameTable_t var_info = 
        {
            .type    = TYPE_VAR,
            .name    = DATA(node).identifier,
            .args    = -1,
            .mem_idx = func_info->mem_idx + STK_SIZE(Local_NameTable)
        };

        Stack_Push(Local_NameTable, var_info);

        return Local_NameTable;
    }

    if ( LEFT(node) )
        Fill_local_NameTable( LEFT(node), Local_NameTable, func_info );

    if ( RIGHT(node) )
        Fill_local_NameTable( RIGHT(node), Local_NameTable, func_info );

    return Local_NameTable;
}

//------------------------------------------------------------------------------------------------------------
#undef PRINT
#undef GET_RAM_IDX
#undef CASE_MATH_OPER