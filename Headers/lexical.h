#ifndef DISK_H
#define DISK_H

#include <stdio.h>

#include "types.h"
#include "utils.h"
#include "tree.h"

enum Token_t {
    _NUMBER_        = -2,
    _VARIABLE_      = -1,
    _UNDEF_TOKEN_   =  0,

    _OPEN_BRACK_    =  1,
    _CLOSE_BRACK_   =  2,
    _BEGIN_OPER_    =  3,
    _END_OPER_      =  4,
    _END_STATEMENT_ =  5,
    _LOG_EQUAL_     =  6,
    _ASSIGNMENT_    =  7,
    _IF_            =  8,
    _ELSE_          =  9,
    _WHILE_         = 10,
    _END_PROGRAM_   = 11
};

struct Token_str;
struct Token_info;
struct Lex_str;
struct Stack_str;

struct Lex_str {
    size_t line;
    size_t pos;
};

struct Token_str {
    Token_t   code;
    char*     name;
    Lex_str   lex_info;
};

struct Token_info {
    const char* key_word; 
    Token_t     code;
    size_t      len;
};

Tree_t*     Create_Tree_from_disk (const char* input_file_name);

Disk_t*     Read_file_to_buffer   (FILE* input_file);
Stack_str*  Create_token_stack    (char* str); 

size_t      Fill_token_stack    (char* str, Stack_str* token_stk);

void Test_Grammar(const char* input_file_name);
#endif