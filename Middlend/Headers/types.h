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

enum is_leaf_checking {
    IS_LEAF  = 1,
    NOT_LEAF = 0
};

enum subtree {
    LEFT_SON  = 0,
    RIGHT_SON = 1
};

enum is_nil {
    NOT_NIL = 0,
    IS_NIL  = 1
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

union  NodeData_t;
struct TreeNode_t;
struct DiskBuf_t;
struct Tree_t;

typedef int        bool_t;
typedef NodeData_t TreeElem_t;
typedef int        TreeErr_t;

union NodeData_t {
    Token_t oper;
    double  num;
    char*   identifier;
};

struct TreeNode_t {
    TreeElem_t  data;
    Data_Type   type;
    TreeNode_t* left;
    TreeNode_t* right;
    TreeNode_t* parent;
};

struct DiskBuf_t {
    char*  buffer;
    size_t size;
};

struct Tree_t {
    TreeNode_t* root;
    DiskBuf_t*  disk;
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

#endif