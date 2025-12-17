#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
  
#include "stack.h"
#include "utils.h"

Stack_str* Stack_Ctor(size_t capacity) 
{
    SAFE_CALLOC(stack,     1,    Stack_str);
    SAFE_CALLOC(data,  capacity, Stack_t);

    STK_DATA(stack)     = data;
    STK_CAPACITY(stack) = capacity;
    STK_SIZE(stack)     = 0;

    return stack;
}

StackErr_t Stack_Dtor(Stack_str* stack) {
    assert(stack);

    STK_SIZE(stack)     = 0;
    STK_CAPACITY(stack) = 0;

    free(STK_DATA(stack));
    free(stack); 

    return STK_NO_ERR;
}

StackErr_t Stack_Push(Stack_str* stack, Stack_t value) {
    assert(stack);

    if (STK_SIZE(stack) >= STK_CAPACITY(stack) - 1)
        Stack_Realloc(stack);

    STK_DATA(stack)[ STK_SIZE(stack) ] = value;

    STK_SIZE(stack)++ ;
    
    return STK_NO_ERR;
}

Stack_t Stack_Pop(Stack_str* stack) { 
    assert(stack);

    if (STK_SIZE(stack) == 0) {
        fprintf(stderr, "%s in %s:%d: stack is empty\n", __func__, __FILE__, __LINE__);
        return STK_DATA(stack)[ STK_SIZE(stack) ];
    }

    STK_SIZE(stack)-- ;

    Stack_t popped_elem = STK_DATA(stack)[ STK_SIZE(stack) ];

    return popped_elem;
}

StackErr_t Stack_Realloc(Stack_str* stack) {
    assert(stack);

    size_t new_capacity = 2 * STK_CAPACITY(stack);

    Stack_t* stk_data_temp = (Stack_t*) realloc(STK_DATA(stack), new_capacity * sizeof(Stack_t));

    if (stk_data_temp == NULL) {                  
        fprintf(stderr, "Reallocation error of stack in %s (%s:%d)\n", __func__, __FILE__, __LINE__); 
        return STK_REALLOC_ERR;
    }

    STK_DATA(stack) = stk_data_temp;

    memset(STK_DATA(stack) + STK_CAPACITY(stack), 0, (new_capacity - STK_CAPACITY(stack)) * sizeof(Stack_t));

    STK_CAPACITY(stack) = new_capacity;

    return STK_NO_ERR;
}

//---------------------------------------------------------------------------------------------------------------------

StackErr_t NameTable_Push_Func(Stack_str* stack, TreeNode_t* func_node) {
    assert(stack);
    assert(func_node);
    assert( TYPE(func_node) == TYPE_FUNC_INIT );

    size_t idx = 0;

    if ( STK_SIZE(stack) != 0 )
        idx = STK_DATA(stack)[ STK_SIZE(stack) - 1 ].mem_idx + STK_DATA(stack)[ STK_SIZE(stack) - 1 ].args;

    int args_count = 1;

    if ( ! LEFT(func_node) ) {
        args_count = 0;
    
    } else {
        while ( TYPE( LEFT(func_node) ) == TYPE_OPER && LEFT(func_node)->data.oper == _COMMA_ )
            args_count++ ;
    }

    NameTable_t func = 
    {
        .type    = TYPE_FUNC,
        .name    = func_node->data.identifier,
        .args    = args_count,
        .mem_idx = idx
    };

    return Stack_Push(stack, func);
}

StackErr_t NameTable_Push_Var(Stack_str* stack, TreeNode_t* var_node) {
    assert(stack);
    assert(var_node);
    assert( TYPE(var_node) == TYPE_VAR_INIT );

    size_t idx = 0;

    if ( STK_SIZE(stack) != 0 )
        idx = STK_DATA(stack)[ STK_SIZE(stack) - 1 ].mem_idx + STK_DATA(stack)[ STK_SIZE(stack) - 1 ].args;

    NameTable_t var = 
    {
        .type    = TYPE_VAR,
        .name    = var_node->data.identifier,
        .args    = 1,
        .mem_idx = idx
    };

    return Stack_Push(stack, var);
}