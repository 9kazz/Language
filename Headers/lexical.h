#ifndef DISK_H
#define DISK_H

#include <stdio.h>

#include "types.h"
#include "utils.h"
#include "tree.h"

enum Token_t {
    _UNDEF_TOKEN_ = -1,
    
    _OPEN_BRACK_  =  0,
    _CLOSE_BRACK_ =  1,
    _LOG_EQUAL_   =  2,
    _ASSIGNMENT_  =  3,
    _IF_          =  4,
    _ELSE_        =  5,
    _WHILE_       =  6,
    _END_         =  7
};

struct Token_str;
struct Token_info;

struct Token_str {
    Token_t   code;
    char*     name;
};

struct Token_info {
    const char* key_word; 
    Token_t     code;
    size_t      len;
};

Tree_t*     Create_Tree_from_disk (const char* input_file_name);

Disk_t*     Read_file_to_buffer   (FILE* input_file);

size_t      Fill_token_stack    (char* str, Token_str* token_arr);

void Test_Grammar(const char* input_file_name);
#endif