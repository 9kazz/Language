#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdio.h>

#include "types.h"

struct Oper_info {
    Operators   code;
    const char* enum_name;
};

struct Type_Info {
    const char* key_word;
    Data_Type   type;
};

extern Oper_info Oper_Info_Arr[];
extern size_t     OPER_COUNT;

extern Type_Info  Type_Arr[];
extern size_t     TYPES_COUNT;

//------------------------------------------------------------------

Operators Find_Oper(char* oper_name);

#endif