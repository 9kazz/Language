#ifndef STACK_H
#define STACK_H

#include <stdio.h>

#include "lexical.h"

typedef Token_str Stack_t;

enum StackErr_t {
    STK_NO_ERR = 0
};

struct Stack_str {
    Stack_t* data;
    size_t   capacity;
    size_t   size;
};

Stack_str* Stack_Ctor      (size_t     size_of_stack);
StackErr_t Stack_Dtor      (Stack_str* stack);
StackErr_t Stack_Dtor_Elem (Stack_t*   stk_elem);

StackErr_t Stack_Push      (Stack_str* stack, Stack_t value);
Stack_t    Stack_Pop       (Stack_str* stack);

StackErr_t Stack_Realloc   (Stack_str* stack);

#define STK_DATA(stack)      stack->data
#define STK_CAPACITY(stack)  stack->capacity
#define STK_SIZE(stack)      stack->size

#endif  