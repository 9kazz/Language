#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

enum Token_t {
    _NUMBER_            = -3,
    _FUNCTION_          = -2,
    _VARIABLE_          = -1,
    _UNDEF_TOKEN_       =  0,
    //-------------------------------------
    _FUNC_INIT_         = 1,
    _VAR_INIT_          = 2,
    _OPEN_BRACK_        = 3,
    _CLOSE_BRACK_       = 4,
    _BEGIN_OPER_        = 5,
    _END_OPER_          = 6,
    _END_STATEMENT_     = 7,
    _COMMA_             = 8,
    _LOG_EQUAL_         = 9,
    _LOG_MORE_          = 10,
    _LOG_LESS_          = 11,
    _ASSIGNMENT_        = 12,
    _IF_                = 13,
    _ELSE_              = 14,
    _WHILE_             = 15,
    _END_PROGRAM_       = 16,
    _MATH_ADD_          = 17,
    _MATH_SUB_          = 18,
    _MATH_MUL_          = 19,
    _MATH_DIV_          = 20,
    _MATH_POW_          = 21,
    _MATH_UNARY_START_  = 22,
    _MATH_SQRT_         = 22,
    _MATH_SIN_          = 23,
    _MATH_COS_          = 24,
    _MATH_TAN_          = 25,
    _MATH_COT_          = 26,
    _MATH_ASIN_         = 27,
    _MATH_ACOS_         = 28,
    _MATH_ATAN_         = 29,
    _MATH_ACOT_         = 30,
    _MATH_SINH_         = 31,
    _MATH_COSH_         = 32,
    _MATH_TANH_         = 33,
    _MATH_COTH_         = 34,
    _MATH_EXP_          = 35,
    _MATH_LN_           = 36,
    _MATH_UNARY_END_    = 36
};

enum Data_Type {
    TYPE_UNKNOWN   = 0, 
    TYPE_OPER      = 1,
    TYPE_NUM       = 2,
    TYPE_VAR       = 3,
    TYPE_FUNC      = 4,
    TYPE_VAR_INIT  = 5,
    TYPE_FUNC_INIT = 6
};

enum Is_leaf_checking {
    IS_LEAF  = 1,
    NOT_LEAF = 0
};

enum ArgsCount {
    ONE_ARG = 1,
    TWO_ARG = 0
};

enum Bools {
    TRUE  = 1,
    FALSE = 0
};

enum Compare {
    LESS  = -1,
    EQUAL = 0,
    MORE  = 1
};

enum NodeSon {
    SON_L = 0,
    SON_R = 1
};

struct TreeNode_t;
struct Disk_t;
struct Tree_t;
struct Token_str;
struct Token_info;
struct Lex_str;
struct Stack_str;

typedef Token_str* TreeElem_t;
typedef int        TreeErr_t;

struct Tree_t {
    TreeNode_t* root;
    Stack_str*  tokens;
};

struct TreeNode_t {
    TreeElem_t  token;
    TreeNode_t* left;
    TreeNode_t* right;
    TreeNode_t* parent;
};

struct Lex_str {
    size_t line;
    size_t pos;
};

struct Token_str {
    Token_t code;
    char*   name;
    double  value;
    Lex_str lex_info;
};

struct Token_info {
    const char* key_word; 
    Token_t     code;
    const char* enum_name;
    size_t      len;
};

struct Disk_t {
    char*  buffer;
    size_t size;
};

#endif 