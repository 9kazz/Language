#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "types.h"

const int POISON = 0xDEADFACE;

enum Func_status {
    END_WITH_SUC = 1
};

size_t    skip_space           (char** str);
Compare   Double_compare       (double num_1, double num_2);

#define SAFE_CALLOC(name, size_of_buf, el_type)                                                             \
    el_type* temp_##name = (el_type*) calloc(size_of_buf, sizeof(el_type));                                 \
                                                                                                            \
    if (temp_##name == NULL)                                                                                \
        fprintf(stderr, "Allocation error of " #name " in %s (%s:%d)\n", __func__, __FILE__, __LINE__);     \
                                                                                                            \
    el_type* name = temp_##name;    
    
#define SAFE_REALLOC(name, size_of_buf, el_type)                                                            \
    el_type* temp_##name = (el_type*) calloc(size_of_buf, sizeof(el_type));                                 \
                                                                                                            \
    if (temp_##name == NULL)                                                                                \
        fprintf(stderr, "Allocation error of " #name " in %s (%s:%d)\n", __func__, __FILE__, __LINE__);     \
                                                                                                            \
    el_type* name = temp_##name;    
    
#define SAFE_FOPEN(name, file, mode)                                                                        \
    FILE* temp_##name = fopen(file, mode);                                                                  \
                                                                                                            \
    if (temp_##name == NULL)                                                                                \
        fprintf(stderr, "File opening error (" #name ") in %s (%s:%d)\n", __func__, __FILE__, __LINE__);    \
                                                                                                            \
    FILE* name = temp_##name; 

#define SAFE_SNPRINTF(str, size, ...)                                                                       \
    int snprintf_check_##str = snprintf(str, size, __VA_ARGS__);                                            \
                                                                                                            \
    if (snprintf_check_##str < 0)                                                                           \
        fprintf(stderr, "snprintf error (" #str ") in %s (%s:%d)\n", __func__, __FILE__, __LINE__);


#define SAFE_FSTAT(describtor, ptr_to_stat_struct)                                                          \
    int stat_check_##file = fstat(describtor, ptr_to_stat_struct);                                          \
                                                                                                            \
    if (stat_check_##file == -1)                                                                            \
        fprintf(stderr, "File stat error in function %s in %s:%d\n", __func__, __FILE__, __LINE__);         \
    

#define SAFE_FREAD(buffer, size_of_elem, count_of_elem, file)                                                                               \
    int fread_check_##file = fread(buffer, size_of_elem, count_of_elem, file);                                                              \
                                                                                                                                            \
    if (fread_check_##file < count_of_elem)                                                                                                 \
        fprintf(stderr, "Warning: error in fread or fread did not read any symbols (%s in %s:%d)\n", __func__, __FILE__, __LINE__);         \

#define XOR(logic_1, logic_2)                                       \
    ( (logic_1) && ! (logic_2) ) || ( ! (logic_1) && (logic_2) )

#define LEFT(node)       node->left
#define RIGHT(node)      node->right
#define PARENT(node)     node->parent

#define NODE_TKN(node)   node->token
#define TKN_VAL(token)   (token)->value
#define TKN_CODE(token)  (token)->code
#define TKN_NAME(token)  (token)->name

#define ROOT(tree)       tree->root
#define TREE_TKN(tree)   tree->tokens

#endif