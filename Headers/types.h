#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

enum Operators {
    OP_UNKNOWN = -1,
    
    OP_ADD   =  0,
    OP_SUB   =  1,
    OP_MUL   =  2,
    OP_DIV   =  3,
    OP_LOG   =  4,
    OP_POW   =  5,
    OP_UNARY =  5, // before OP_UNAR -- binary operators, after OP_UNAR -- unary operators
    OP_SQRT  =  6,
    OP_SIN   =  7,
    OP_COS   =  8,
    OP_TAN   =  9,
    OP_COT   = 10,
    OP_ASIN  = 11,
    OP_ACOS  = 12,
    OP_ATAN  = 13,
    OP_ACOT  = 14,
    OP_SINH  = 15,
    OP_COSH  = 16,
    OP_TANH  = 17,
    OP_COTH  = 18,
    OP_EXP   = 19,
    OP_LN    = 20
};

enum Data_Type {
    TYPE_NOT_NODE = -1,
    TYPE_UNKNOWN  = 0, 
    TYPE_OPER     = 1,
    TYPE_NUM      = 2,
    TYPE_VAR      = 3
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

union  NodeData_t;

struct TreeNode_t;
struct Disk_t;
struct Tree_t;

struct OperArr_t;
struct VarArr_t;

typedef NodeData_t TreeElem_t;
typedef int        TreeErr_t;

struct Tree_t {
    TreeNode_t* root;
    Disk_t*     disk;
};

union NodeData_t {
    Operators oper;
    double    num;
    VarArr_t* var;
};

struct TreeNode_t {
    TreeElem_t  data;
    Data_Type   type;
    TreeNode_t* left;
    TreeNode_t* right;
    TreeNode_t* parent;
};

struct Disk_t {
    char*  buffer;
    size_t size;
};

struct OperArr_t {
    char*     name;
    char*     symbol;
    Operators code;
};

struct VarArr_t {
    char*  name;
    double value;
};

extern OperArr_t Oper_info_arr[];
extern VarArr_t  Var_info_arr[];

extern const int OPER_COUNT;
extern const int VAR_COUNT;

#endif 