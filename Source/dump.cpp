#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "Logfiles.h" 
#include "utils.h"
#include "tree.h"
#include "dump.h"
#include "Tokens.h"
#include "stack.h"

#define TYPE(node)       Node_Type(node)

#define LEFT(node)       node->left
#define RIGHT(node)      node->right
#define ROOT(tree)       tree->root

#define PRINTF_OUTPUT(format, ...)  fprintf(output_file, format, ##__VA_ARGS__);
#define PRINTF_LOG(format, ...)     fprintf(TreeLog, format, ##__VA_ARGS__);

//----------------------------------------------------------------------------------------------------------------------

TreeErr_t Graphic_Dump_Tree(const TreeNode_t* root, const char* dump_info) {
    assert(root);

    static size_t dump_num = 1;

    static const size_t    MAX_STR_LEN  = 256;
    char image_name_str   [MAX_STR_LEN] = {0};
    char create_image_CMD [MAX_STR_LEN] = {0};

    SAFE_SNPRINTF(image_name_str, MAX_STR_LEN, "dump_files/image_%d.png", dump_num); 

    SAFE_FOPEN(graphviz_file, "dump_files/graphviz.txt", "w")

    Print_Tree_to_Graphviz(root, graphviz_file);

    fclose(graphviz_file);

    SAFE_SNPRINTF(create_image_CMD, MAX_STR_LEN, "dot -Tpng dump_files/graphviz.txt -o %s", image_name_str);

    system(create_image_CMD);

    Dump_Node_to_HTML(root, image_name_str, dump_info);

    ++dump_num;

    return END_WITH_SUC;
}

TreeErr_t Print_Tree_to_Graphviz(const TreeNode_t* root, FILE* output_file) {
    assert(root);
    assert(output_file);

    PRINTF_OUTPUT("digraph Tree {\n");
    PRINTF_OUTPUT("\trankdir = TB;\n");
    PRINTF_OUTPUT("\tnode [" SHAPE STYLE " fillcolor = red, color = darkred, width = 1, height = 1];\n");
    PRINTF_OUTPUT("\tedge [color = \"black\"];\n\n");
    
    Print_Node_to_Graphviz(root, output_file);

    PRINTF_OUTPUT("\n}\n");

    return END_WITH_SUC;
}

TreeErr_t Print_Node_to_Graphviz(const TreeNode_t* node, FILE* output_file) {
    assert(node);
    assert(output_file);

    static int node_rank = 0;
    ++node_rank;
    
    if (node->left) 
        Print_Node_to_Graphviz(node->left, output_file);
    if (node->right)
        Print_Node_to_Graphviz(node->right, output_file);

    switch ( TYPE(node) )
    {
    case TYPE_OPER:
        if ( Is_Leaf_Node( (TreeNode_t*) node) == IS_LEAF ) {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: OPER | key word: '%s' | code: %s (%d) | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n",   node, Token_Info_Arr[node->token->code - 1].key_word, Token_Info_Arr[node->token->code - 1].enum_name, Token_Info_Arr[node->token->code - 1].code, node, PARENT(node), node_rank);
        } else if ( RIGHT(node) == NULL && LEFT(node) != NULL) {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: OPER | key word: '%s' | code: %s (%d) | ptr: %p | par: %p | {<left> %p | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n",  node, Token_Info_Arr[node->token->code - 1].key_word, Token_Info_Arr[node->token->code - 1].enum_name, Token_Info_Arr[node->token->code - 1].code, node, PARENT(node), LEFT(node), node_rank);
        } else {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: OPER | key word: '%s' | code: %s (%d) | ptr: %p | par: %p | {<left> %p | <right> %p}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n", node, Token_Info_Arr[node->token->code - 1].key_word, Token_Info_Arr[node->token->code - 1].enum_name, Token_Info_Arr[node->token->code - 1].code, node, PARENT(node), LEFT(node), RIGHT(node), node_rank);
        }
        break;
    
    case TYPE_NUM:
        if ( Is_Leaf_Node( (TreeNode_t*) node) == IS_LEAF ) {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: NUM | val: %lg | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_BLUE ", color = darkblue]\n",   node, node->token->value, node, PARENT(node), node_rank);
        } else {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: NUM | val: %lg | ptr: %p | par: %p | {<left> %p | <right> %p}}\", rank = %d, fillcolor = " BRIGHT_BLUE ", color = darkblue]\n", node, node->token->value, node, PARENT(node), LEFT(node), RIGHT(node), node_rank);
        }
        break;
    
    case TYPE_VAR:
        if ( Is_Leaf_Node( (TreeNode_t*) node) == IS_LEAF ) {
            
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: VAR | name: '%s' | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " PASTEL_PINK ", color = " DARK_PINK "]\n",   node, node->token->name, node, PARENT(node), node_rank);
        } else {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: VAR | name: '%s' | ptr: %p | par: %p | {<left> %p | <right> %p}}\", rank = %d, fillcolor = " PASTEL_PINK ", color = " DARK_PINK "]\n", node, node->token->name, node, LEFT(node), RIGHT(node), PARENT(node), node_rank);
        }
        break;

    default:
        break;
    }

    if (node->left) {
        PRINTF_OUTPUT("\tnode_%d: <left> -> node_%d [dir = both]\n", node, node->left);
    } if (node->right) {
        PRINTF_OUTPUT("\tnode_%d: <right> -> node_%d [dir = both]\n", node, node->right);
    }
    
    --node_rank;
    return END_WITH_SUC;
}

TreeErr_t Dump_Node_to_HTML(const TreeNode_t* node, const char* image_file_name, const char* dump_info) {
    assert(node);

    PRINTF_LOG("<pre>\n");
    PRINTF_LOG("\t<h2> Dump of tree (%s): </h2>\n", dump_info);
    PRINTF_LOG("\t<h2> <span style = \"color: blue;\"> No critical errors </span></h2>\n");

    PRINTF_LOG("\n<h3>Image:</h3>\n");
    PRINTF_LOG("\t<img src = %s width = \"600\" height = \"600\">\n\n", image_file_name);

    PRINTF_LOG("</pre>");

    return END_WITH_SUC;
}

TreeErr_t Dump_disk_buffer_html (char* disk_buffer, size_t cur_pos, size_t size_of_file) {
    assert(disk_buffer);

    PRINTF_LOG("<h1>Dump of disk_buffer</h1>\n");

    PRINTF_LOG("<span style = \"color: grey;\">");

    for (size_t pos = 0; pos < cur_pos; pos++)
        PRINTF_LOG("%c", disk_buffer[pos]);

    PRINTF_LOG("</span>");

    PRINTF_LOG("<span style = \"color: orange;\">%d</span>", disk_buffer[cur_pos]);

    PRINTF_LOG("<span style = \"color: blue;\">");
    
    for (size_t pos = cur_pos + 1; pos < size_of_file; pos++)
        PRINTF_LOG("%c", disk_buffer[pos]);

    PRINTF_LOG("</span>");

    PRINTF_LOG("</h1>\n");

    return END_WITH_SUC;
}

TreeErr_t Dump_disk_buffer (char* disk_buffer, size_t cur_pos, size_t size_of_file) {
    assert(disk_buffer);

    fprintf(stdout, "\"");

    for (size_t pos = 0; pos < cur_pos; pos++) {
        if (disk_buffer[pos] == '\0') fprintf(stdout, T_RED "0" T_RESET);
        else                          fprintf(stdout, T_GREY "%c" T_RESET, disk_buffer[pos]);
    }

    if (disk_buffer[cur_pos] == '\0') fprintf(stdout, T_RED "0" T_RESET);
    else                              fprintf(stdout, T_BG_YELLOW "%c" T_BG_RESET, disk_buffer[cur_pos]);
    
    for (size_t pos = cur_pos + 1; pos < size_of_file; pos++) {
        if (disk_buffer[pos] == '\0') fprintf(stdout, T_RED "0" T_RESET);
        else                          fprintf(stdout, T_BRIGHT_BLUE "%c" T_RESET, disk_buffer[pos]);
    }

    fprintf(stdout, "\"\n");

    return END_WITH_SUC;
}

//----------------------------------------------------------------------------------------------------------------------

#undef PRINTF_OUTPUT
#undef PRINTF_LOG