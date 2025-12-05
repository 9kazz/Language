#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
  
#include "utils.h"
#include "stack.h"

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

    for (size_t idx = 0; idx < STK_SIZE(stack); idx++) 
        Stack_Dtor_Elem( STK_DATA(stack) + idx );

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

    Stack_Dtor_Elem( STK_DATA(stack) + STK_SIZE(stack) );

    return popped_elem;
}

StackErr_t Stack_Realloc(Stack_str* stack) {
    assert(stack);

    size_t new_capacity = 2 * STK_CAPACITY(stack);

    SAFE_REALLOC(realloc_data, new_capacity, Stack_t);

    STK_DATA(stack) = realloc_data;

    STK_CAPACITY(stack) = new_capacity;
    
    return STK_NO_ERR;
}

StackErr_t Stack_Dtor_Elem(Stack_t* stk_elem) {
    assert(stk_elem);
    
    if ( stk_elem->name != NULL )
        free(stk_elem->name);

    return STK_NO_ERR;
}