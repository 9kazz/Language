#ifndef TYPES_H
#define TYPES_H 

#include <stdio.h>

enum Operators {
    _UNDEF_TOKEN_       =  0,
//-------------------------------------
    _OPEN_BRACK_        =  1,
    _CLOSE_BRACK_       =  2,
    _BEGIN_OPER_        =  3,
    _END_OPER_          =  4,
    _END_STATEMENT_     =  5,
    _LOG_EQUAL_         =  6,
    _ASSIGNMENT_        =  7,
    _IF_                =  8,
    _ELSE_              =  9,
    _WHILE_             = 10,
    _END_PROGRAM_       = 11,
//-------------------------------------
    _MATH_ADD_          = 12,
    _MATH_SUB_          = 13,
    _MATH_MUL_          = 14,
    _MATH_DIV_          = 15,
    _MATH_POW_          = 16,

    _MATH_UNARY_START_  = 17,
    _MATH_SQRT_         = 17,
    _MATH_SIN_          = 18,
    _MATH_COS_          = 19,
    _MATH_TAN_          = 20,
    _MATH_COT_          = 21,
    _MATH_ASIN_         = 22,
    _MATH_ACOS_         = 23,
    _MATH_ATAN_         = 24,
    _MATH_ACOT_         = 25,
    _MATH_SINH_         = 26,
    _MATH_COSH_         = 27,
    _MATH_TANH_         = 28,
    _MATH_COTH_         = 29,
    _MATH_EXP_          = 30,
    _MATH_LN_           = 31,
    _MATH_UNARY_END_    = 31
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
    TYPE_UNKNOWN = 0,
    TYPE_OPER    = 1,
    TYPE_NUM     = 2,
    TYPE_VAR     = 3,
    TYPE_FUNC    = 4
};

union  NodeData_t;
struct TreeNode_t;
struct DiskBuf_t;
struct Tree_t;

typedef int        bool_t;
typedef NodeData_t TreeElem_t;
typedef int        TreeErr_t;

union NodeData_t {
    Operators oper;
    double    num;
    char*     identifier;
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

#endif