#ifndef DUMP_H
#define DUMP_H

#include <stdio.h>

#include "Logfiles.h"
#include "types.h"

#define SHAPE   "shape = Mrecord, "
#define STYLE   "style = \"filled\", "

                /*COLORS*/

#define BRIGHT_GREEN    "\"#C0FFC0\""                
#define BRIGHT_RED      "\"#FFC0C0\""
#define BRIGHT_BLUE     "\"#C0C0FF\""
#define BRIGHT_YELLOW   "\"#ffff94ff\""
#define YELLOW          "\"#cdcd00ff\""
#define PASTEL_PINK     "\"#ff83a0ff\""
#define DARK_PINK       "\"#a40e5eff\""
#define ORANGE          "\"#fcc95cff\""
#define DARK_ORANGE     "\"#FF8C00\""
#define LIGHT_BROWN     "\"#D2B48C\""
#define DARK_BROWN      "\"#8B4513\"" 
#define PURPLE          "\"#efa0ffff\""
#define DARK_PURPLE     "\"#8A2BE2\""      

#define T_BLUE          "\033[34m"
#define T_BRIGHT_BLUE   "\033[36m"
#define T_RED           "\033[31m"
#define T_YELLOW        "\033[93m"
#define T_GREEN         "\033[32m"
#define T_RESET         "\033[0m"
#define T_GREY          "\033[90m"
#define T_BLACK         "\033[30m"

#define T_BG_BLUE          "\033[44m"    
#define T_BG_BRIGHT_BLUE   "\033[46m"    
#define T_BG_RED           "\033[41m"    
#define T_BG_YELLOW        "\033[43m"   
#define T_BG_GREEN         "\033[42m"    
#define T_BG_GREY          "\033[100m"   
#define T_BG_WHITE         "\033[47m"    
#define T_BG_RESET         "\033[49m"  

const int MAX_DUMP_INFO_LEN = 256;

#define DUMP_BEFORE(tree)                                                                                                                        \
    {                                                                                                                                            \
    static char dump_info_str[MAX_DUMP_INFO_LEN] = {0};                                                                                          \
    snprintf(dump_info_str, MAX_DUMP_INFO_LEN, "Before <span style = \"color: blue;\"> %s </span> in %s:%d", __func__, __FILE__, __LINE__);      \
    Graphic_Dump_Node(tree, dump_info_str);                                                                                                      \
    }
    
#define DUMP_AFTER(tree)                                                                                                                         \
    {                                                                                                                                            \
    static char dump_info_str[MAX_DUMP_INFO_LEN] = {0};                                                                                          \
    snprintf(dump_info_str, MAX_DUMP_INFO_LEN, "After <span style = \"color: blue;\"> %s </span> in %s:%d", __func__, __FILE__, __LINE__);       \
    Graphic_Dump_Node(tree, dump_info_str);                                                                                                      \
    }

#define DUMP_ERROR(tree)                                                                                                                         \
    {                                                                                                                                            \
    static char dump_info_str[MAX_DUMP_INFO_LEN] = {0};                                                                                          \
    snprintf(dump_info_str, MAX_DUMP_INFO_LEN, "<span style = \"color: red;\"> ERROR of %s </span> in %s:%d", __func__, __FILE__, __LINE__);     \
    Graphic_Dump_Node(tree, dump_info_str);                                                                                                      \
    }

TreeErr_t Graphic_Dump_Tree         (const TreeNode_t* root, const char* dump_info);
TreeErr_t Print_Tree_to_Graphviz    (const TreeNode_t* root, FILE* output_file);
TreeErr_t Print_Node_to_Graphviz    (const TreeNode_t* node, FILE* output_file);
TreeErr_t Dump_Node_to_HTML         (const TreeNode_t* node, const char* image_file_name, const char* dump_info);

TreeErr_t Dump_Node_preorder        (const TreeNode_t* node, FILE* output_file);

TreeErr_t Print_Space               (size_t spaces_count, FILE* output_file);

#endif