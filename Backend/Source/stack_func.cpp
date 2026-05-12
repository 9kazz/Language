#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
  
#include "stack_func.h"
#include "stack_var.h"
#include "utils.h"

Fstack_str* func_Stack_Ctor(size_t capacity) 
{
    SAFE_CALLOC(stack,     1,    Fstack_str);
    SAFE_CALLOC(data,  capacity, Fstack_t);

    STK_DATA(stack)     = data;
    STK_CAPACITY(stack) = capacity;
    STK_SIZE(stack)     = 0;

    return stack;
}

StackErr_t func_Stack_Dtor(Fstack_str* stack) {
    assert(stack);

    for (size_t stk_el = 0;  stk_el < STK_SIZE(stack);  stk_el++) 
    {
        var_Stack_Dtor(DATA(stack)[stk_el].vars_stk);
    }
    
    STK_SIZE(stack)     = 0;
    STK_CAPACITY(stack) = 0;

    free(STK_DATA(stack));
    free(stack); 

    return STK_NO_ERR;
}

StackErr_t func_Stack_Push(Fstack_str* stack, Fstack_t value) {
    assert(stack);

    if (STK_SIZE(stack) >= STK_CAPACITY(stack) - 1)
        func_Stack_Realloc(stack);

    STK_DATA(stack)[ STK_SIZE(stack) ] = value;

    STK_SIZE(stack)++ ;
    
    return STK_NO_ERR;
}

Fstack_t func_Stack_Pop(Fstack_str* stack) { 
    assert(stack);

    if (STK_SIZE(stack) == 0) {
        fprintf(stderr, "%s in %s:%d: stack is empty\n", __func__, __FILE__, __LINE__);
        return STK_DATA(stack)[ STK_SIZE(stack) ];
    }

    STK_SIZE(stack)-- ;

    Fstack_t popped_elem = STK_DATA(stack)[ STK_SIZE(stack) ];
    var_Stack_Dtor(popped_elem.vars_stk);
    
    return popped_elem;
}

StackErr_t func_Stack_Realloc(Fstack_str* stack) {
    assert(stack);

    size_t new_capacity = 2 * STK_CAPACITY(stack);

    Fstack_t* stk_data_temp = (Fstack_t*) realloc(STK_DATA(stack), new_capacity * sizeof(Fstack_t));

    if (stk_data_temp == NULL) {                  
        fprintf(stderr, "Reallocation error of stack in %s (%s:%d)\n", __func__, __FILE__, __LINE__); 
        return STK_REALLOC_ERR;
    }

    STK_DATA(stack) = stk_data_temp;

    memset(STK_DATA(stack) + STK_CAPACITY(stack), 0, (new_capacity - STK_CAPACITY(stack)) * sizeof(Fstack_t));

    STK_CAPACITY(stack) = new_capacity;

    return STK_NO_ERR;
}