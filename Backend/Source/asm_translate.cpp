#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "stack_func.h"
#include "stack_var.h"
#include "asm_translate.h"
#include "nametable.h"

//---------------------------------------------------------------------------------------------------------------------
#define PRINT(format, ...)  fprintf(Asm_file, format, ##__VA_ARGS__);

#define CASE_MATH_OPER(oper)                                              \
    case _MATH_##oper##_:                                                 \
        Asm_Translate( LEFT(cur_node),  func_NameTable, var_NameTable );  \
        Asm_Translate( RIGHT(cur_node), func_NameTable, var_NameTable );  \
        PRINT(#oper "\n");                                                \
        return cur_node;

#define CASE_LOG_OPER(oper, asm_cmd)                                      \
    case _LOG_##oper##_:                                                  \
        Asm_Translate( LEFT(cur_node),  func_NameTable, var_NameTable );  \
        Asm_Translate( RIGHT(cur_node), func_NameTable, var_NameTable );  \
                                                                          \
        PRINT(#asm_cmd " :true_" #oper "_%d\n", cur_node);                \
        PRINT("OUT\n");                                                   \
        PRINT("OUT\n");                                                   \
        PRINT("PUSH 0\n");                                                \
        PRINT("JMP :false_" #oper "_%d\n", cur_node);                     \
                                                                          \
        PRINT("true_" #oper "_%d:\n", cur_node);                          \
        PRINT("OUT\n");                                                   \
        PRINT("OUT\n");                                                   \
        PRINT("PUSH 1\n");                                                \
        PRINT("false_" #oper "_%d:\n", cur_node);                         \
        return cur_node;
//---------------------------------------------------------------------------------------------------------------------

void Asm_Main_Function(TreeNode_t* tree_root) {
    assert(tree_root);

    Fstack_str* func_NameTable = Create_func_NameTable();

    Asm_Translate(tree_root, func_NameTable, STK_DATA(func_NameTable)[0].vars_stk);
    PRINT("HLT\n");

    Print_NameTable(func_NameTable);

    func_Stack_Dtor(func_NameTable);
}

TreeNode_t* Asm_Translate(TreeNode_t* cur_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable) {
    assert(func_NameTable);
    assert(var_NameTable);

    if ( ! cur_node )
        return NULL;

    switch( TYPE(cur_node) )
    {
    case TYPE_OPER:      Asm_Translate_Oper(cur_node, func_NameTable, var_NameTable);        break;
    case TYPE_NUM:       Asm_Translate_Num(cur_node);                                        break;
    case TYPE_VAR:       Asm_Translate_Var(cur_node, var_NameTable);                         break;
    case TYPE_FUNC:      Asm_Translate_Func_Call(cur_node, func_NameTable, var_NameTable);   break;
    case TYPE_VAR_INIT:  Add_Var_to_NameTable(cur_node, var_NameTable);                      break;
    case TYPE_FUNC_INIT: Asm_Translate_Func_Init (cur_node, func_NameTable, var_NameTable);  break;
        
    default:
        fprintf(stderr, "%s at %s:%d: unknown type\n", __func__, __FILE__, __LINE__);
        break;
    }

    return NULL;
}

TreeNode_t* Asm_Translate_Oper(TreeNode_t* cur_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable) {
    assert(cur_node);
    assert(func_NameTable);
    assert(var_NameTable);
    assert( TYPE(cur_node) == TYPE_OPER );

    switch ( DATA(cur_node).oper )
    {
    case _END_STATEMENT_: case _COMMA_:
               Asm_Translate( LEFT(cur_node),  func_NameTable, var_NameTable );
        return Asm_Translate( RIGHT(cur_node), func_NameTable, var_NameTable );
    
    case _ASSIGNMENT_: 
    {
        if ( TYPE( LEFT(cur_node) ) == TYPE_VAR_INIT )
            Asm_Translate( LEFT(cur_node), func_NameTable, var_NameTable );

        size_t var_idx = Find_Var_in_NameTable( var_NameTable, DATA( LEFT(cur_node) ).identifier );

        Asm_Translate( RIGHT(cur_node), func_NameTable, var_NameTable );

        PRINT("PUSH %d ; [%s] assignment\n", var_idx, DATA( LEFT(cur_node) ).identifier);
        PRINT("PUSHR RAX\n");
        PRINT("ADD\n");
        PRINT("POPR RBX\n");
        PRINT("POPM [RBX]\n");

        return cur_node;
    }

    case _IF_:
        PRINT("PUSH 0 ; if\n");
        Asm_Translate( LEFT(cur_node), func_NameTable, var_NameTable );

        PRINT("JE :if_false%d\n", cur_node);
        PRINT("OUT\n");
        PRINT("OUT\n");

        Asm_Translate( RIGHT(cur_node), func_NameTable, var_NameTable );

        PRINT("JMP :if_true%d\n", cur_node);
        PRINT("if_false%d:\n", cur_node);
        PRINT("OUT\n");
        PRINT("OUT\n");
        PRINT("if_true%d:\n", cur_node);

        return cur_node;

    case _WHILE_:
        PRINT("start_while_%d:\n", cur_node);

        PRINT("PUSH 0\n");
        Asm_Translate( LEFT(cur_node), func_NameTable, var_NameTable );
        PRINT("JE :end_while_%d\n", cur_node);

        PRINT("OUT\n");
        PRINT("OUT\n");
        Asm_Translate( RIGHT(cur_node), func_NameTable, var_NameTable );
        PRINT("JMP :start_while_%d\n", cur_node);

        PRINT("end_while_%d:\n", cur_node);
        PRINT("OUT\n");
        PRINT("OUT\n");

        return cur_node;

    case _RETURN_:
        Asm_Translate( LEFT(cur_node), func_NameTable, var_NameTable );
        PRINT("RET\n");
        return cur_node;

    case _PRINT_:
        Asm_Translate( LEFT(cur_node), func_NameTable, var_NameTable );
        PRINT("PRNT\n");
        return cur_node;

    case _DRAW_:        
        Asm_Translate( LEFT(cur_node), func_NameTable, var_NameTable );
        PRINT("POPR RCX\n");
        PRINT("PUSH 35\n");        
        PRINT("POPM [RCX]\n");   
        return cur_node;     

    CASE_LOG_OPER(EQUAL,       JE);
    CASE_LOG_OPER(LESS,        JB);
    CASE_LOG_OPER(MORE,        JA);
    CASE_LOG_OPER(NOT_EQUAL,   JNE);
    CASE_LOG_OPER(LESS_EQUAL,  JBE);
    CASE_LOG_OPER(MORE_EQUAL,  JAE);

    CASE_MATH_OPER(ADD)
    CASE_MATH_OPER(SUB)
    CASE_MATH_OPER(MUL)
    CASE_MATH_OPER(DIV)
    CASE_MATH_OPER(SQRT)

    default:
        fprintf(stderr, "%s at %s:%d: unknown oper %d\n", __func__, __FILE__, __LINE__, DATA(cur_node).oper);
        break;
    }
    
    return NULL;
}

TreeNode_t* Asm_Translate_Func_Init(TreeNode_t* func_decl_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable) {
    assert(func_decl_node);
    assert(func_NameTable);
    assert(var_NameTable);
    assert( TYPE(func_decl_node) == TYPE_FUNC_INIT );

    Vstack_str* loc_var_NameTable = Add_Func_to_NameTable(func_decl_node, func_NameTable);

    PRINT("JMP :skip_func_init_%s\n", DATA(func_decl_node).identifier);
    PRINT("%s:\n", DATA(func_decl_node).identifier);

    Asm_Translate( RIGHT(func_decl_node), func_NameTable, loc_var_NameTable);

    // size_t vars_amount_in_cur_func = 0;

    // if ( STK_DATA(func_NameTable)[0].vars_stk == var_NameTable ) {
    //     vars_amount_in_cur_func = INIT_MAIN_VARS_COUNT;    
    // } else {
    //     vars_amount_in_cur_func = STK_SIZE(var_NameTable);
    // }
    
    // PRINT("PUSHR RAX ; call func [%s]\n", DATA(func_decl_node).identifier);
    // PRINT("PUSH %d ; count of vars in cur func\n", vars_amount_in_cur_func);
    // PRINT("SUB\n");
    // PRINT("POPR RAX ; new RAM pointer\n");

    PRINT("skip_func_init_%s:\n", DATA(func_decl_node).identifier);

    return func_decl_node;
}

TreeNode_t* Asm_Translate_Var(TreeNode_t* var_node, Vstack_str* var_NameTable) {
    assert(var_node);
    assert(var_NameTable);
    assert( TYPE(var_node) == TYPE_VAR );
    
    size_t relative_idx = Find_Var_in_NameTable(var_NameTable, DATA(var_node).identifier);

    PRINT("PUSHR RAX ; var [%s]\n", DATA(var_node).identifier);
    PRINT("PUSH %d ; relative_idx\n", relative_idx);
    PRINT("ADD\n");
    PRINT("POPR RBX\n");
    PRINT("PUSHM [RBX]\n");

    return var_node;
}

TreeNode_t* Asm_Translate_Num(TreeNode_t* num_node) {
    assert(num_node);
    assert( TYPE(num_node) == TYPE_NUM );

    PRINT("PUSH %lg ; num\n", DATA(num_node).num);

    return num_node;
}

TreeNode_t* Asm_Translate_Func_Call(TreeNode_t* func_call_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable) {
    assert(func_call_node);
    assert(func_NameTable);
    assert(var_NameTable);
    assert( TYPE(func_call_node) == TYPE_FUNC );

    Init_Func_Params(func_call_node, func_NameTable, var_NameTable);

    PRINT("CALL :%s ; function call\n", DATA(func_call_node).identifier);

    Relocate_RAM_ptr(BACK, func_call_node, func_NameTable, var_NameTable);

    return func_call_node; 
}

size_t Relocate_RAM_ptr(direction reloc_dir, TreeNode_t* node, Fstack_str* func_NameTable, Vstack_str* var_NameTable) {
    assert(node);
    assert(func_NameTable);
    assert(var_NameTable); 

    size_t vars_amount_in_cur_func = 0;

    if ( STK_DATA(func_NameTable)[0].vars_stk == var_NameTable ) {
        vars_amount_in_cur_func = INIT_MAIN_VARS_COUNT;    
    } else {
        vars_amount_in_cur_func = STK_SIZE(var_NameTable);
    }
    
    PRINT("PUSHR RAX ; call func [%s]\n", DATA(node).identifier);
    PRINT("PUSH %d ; count of vars in cur func\n", vars_amount_in_cur_func);

    if (reloc_dir == FORWARD) {
        PRINT("ADD\n");
    } else {
        PRINT("SUB\n");
    }

    PRINT("POPR RAX ; new RAM pointer\n");

    return vars_amount_in_cur_func;
}

TreeNode_t* Init_Func_Params(TreeNode_t* func_call_node, Fstack_str* func_NameTable, Vstack_str* var_NameTable) {
    assert(func_call_node);
    assert(func_NameTable);
    assert(var_NameTable);
    assert( TYPE(func_call_node) == TYPE_FUNC );

    if ( ! LEFT(func_call_node) )
        return NULL;

    size_t      param_counter = 1;
    TreeNode_t* temp_node_ptr = LEFT(func_call_node);

    while ( TYPE(temp_node_ptr) == TYPE_OPER && DATA(temp_node_ptr).oper == _COMMA_ ) 
    {
        Asm_Translate( RIGHT(temp_node_ptr), func_NameTable, var_NameTable);

        temp_node_ptr = LEFT(temp_node_ptr);
        param_counter++ ;
    }

    Asm_Translate(temp_node_ptr, func_NameTable, var_NameTable);

    Relocate_RAM_ptr(FORWARD, func_call_node, func_NameTable, var_NameTable);

    while (param_counter > 0)
    {
        PRINT("PUSHR RAX ; %d parametr\n", param_counter - 1);
        PRINT("PUSH %d\n", param_counter - 1);
        PRINT("ADD\n");
        PRINT("POPR RBX\n");
        PRINT("POPM [RBX]\n");

        param_counter-- ;
    }

    return func_call_node;
}

//---------------------------------------------------------------------------------------------------------------------
#undef PRINT