#ifndef STACK_VAR_H
#define STACK_VAR_H

#include <stdio.h>

#include "types.h"
#include "stack_func.h"

Vstack_str* var_Stack_Ctor      (size_t     size_of_stack);
StackErr_t  var_Stack_Dtor      (Vstack_str* stack);

StackErr_t  var_Stack_Push      (Vstack_str* stack, Vstack_t value);
Vstack_t    var_Stack_Pop       (Vstack_str* stack);

StackErr_t  var_Stack_Realloc   (Vstack_str* stack);

#endif  