#ifndef STACK_FUNC_H
#define STACK_FUNC_H

#include <stdio.h>

#include "types.h"

enum StackErr_t {
    STK_NO_ERR      = 0,
    STK_REALLOC_ERR = 0
};

Fstack_str* func_Stack_Ctor     (size_t      size_of_stack);
StackErr_t  func_Stack_Dtor     (Fstack_str* stack);

StackErr_t  func_Stack_Push     (Fstack_str* stack, Fstack_t value);
Fstack_t    func_Stack_Pop      (Fstack_str* stack);

StackErr_t  func_Stack_Realloc  (Fstack_str* stack);

#define STK_DATA(stack)      (stack)->data
#define STK_CAPACITY(stack)  (stack)->capacity
#define STK_SIZE(stack)      (stack)->size

#endif  