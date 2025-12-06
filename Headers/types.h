#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>

enum Data_Type {
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

struct TreeNode_t;
struct Disk_t;
struct Tree_t;

struct OperArr_t;
struct VarArr_t;

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

struct Disk_t {
    char*  buffer;
    size_t size;
};

#endif 