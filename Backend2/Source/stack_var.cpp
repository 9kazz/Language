#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
  
#include "stack_var.h"
#include "utils.h"

Vstack_str* var_Stack_Ctor(size_t capacity) 
{
    SAFE_CALLOC(stack,     1,    Vstack_str);
    SAFE_CALLOC(data,  capacity, Vstack_t);

    STK_DATA(stack)     = data;
    STK_CAPACITY(stack) = capacity;
    STK_SIZE(stack)     = 0;

    return stack;
}

StackErr_t var_Stack_Dtor(Vstack_str* stack) {
    assert(stack);

    STK_SIZE(stack)     = 0;
    STK_CAPACITY(stack) = 0;

    free(STK_DATA(stack));
    free(stack); 

    return STK_NO_ERR;
}

StackErr_t var_Stack_Push(Vstack_str* stack, Vstack_t value) {
    assert(stack);

    if (STK_SIZE(stack) >= STK_CAPACITY(stack) - 1)
        var_Stack_Realloc(stack);

    STK_DATA(stack)[ STK_SIZE(stack) ] = value;

    STK_SIZE(stack)++ ;
    
    return STK_NO_ERR;
}

Vstack_t var_Stack_Pop(Vstack_str* stack) { 
    assert(stack);

    if (STK_SIZE(stack) == 0) {
        fprintf(stderr, "%s in %s:%d: stack is empty\n", __func__, __FILE__, __LINE__);
        return STK_DATA(stack)[ STK_SIZE(stack) ];
    }

    STK_SIZE(stack)-- ;

    Vstack_t popped_elem = STK_DATA(stack)[ STK_SIZE(stack) ];

    return popped_elem;
}

StackErr_t var_Stack_Realloc(Vstack_str* stack) {
    assert(stack);

    size_t new_capacity = 2 * STK_CAPACITY(stack);

    Vstack_t* stk_data_temp = (Vstack_t*) realloc(STK_DATA(stack), new_capacity * sizeof(Vstack_t));

    if (stk_data_temp == NULL) {                  
        fprintf(stderr, "Reallocation error of stack in %s (%s:%d)\n", __func__, __FILE__, __LINE__); 
        return STK_REALLOC_ERR;
    }

    STK_DATA(stack) = stk_data_temp;

    memset(STK_DATA(stack) + STK_CAPACITY(stack), 0, (new_capacity - STK_CAPACITY(stack)) * sizeof(Vstack_t));

    STK_CAPACITY(stack) = new_capacity;

    return STK_NO_ERR;
}