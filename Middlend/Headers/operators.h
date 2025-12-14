#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdio.h>

#include "types.h"

struct Token_info {
    const char* key_word; 
    Token_t     code;
    const char* enum_name;
    size_t      len;
};

struct Type_Info {
    const char* key_word;
    Data_Type   type;
};

extern Token_info Token_Info_Arr[];
extern size_t     OPER_COUNT;

extern Type_Info  Type_Arr[];
extern size_t     TYPES_COUNT;

//------------------------------------------------------------------

Token_t Find_Oper(char* oper_name);

#endif