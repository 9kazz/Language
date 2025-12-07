#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "types.h"
#include "Logfiles.h" 
#include "utils.h"
#include "tree.h"
#include "dump.h"

#define TYPE(node)       Node_Type(node)

#define LEFT(node)       node->left
#define RIGHT(node)      node->right
#define ROOT(tree)       tree->root

#define PRINTF_LOG(format, ...)  fprintf(output_file, format, ##__VA_ARGS__);

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

    PRINTF_LOG("digraph Tree {\n");
    PRINTF_LOG("\trankdir = TB;\n");
    PRINTF_LOG("\tnode [" SHAPE STYLE " fillcolor = red, color = darkred, width = 1, height = 1];\n");
    PRINTF_LOG("\tedge [color = \"black\"];\n\n");

    Print_Node_to_Graphviz(root, output_file);

    PRINTF_LOG("\n}\n");

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
            PRINTF_LOG("\tnode_%d [label = \" { type: OPER | val: %s | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n",   node, Oper_info_arr[ D_OPER(node) ].name,  node, PARENT(node), node_rank);
        } else if ( RIGHT(node) == NULL && LEFT(node) != NULL) {
            PRINTF_LOG("\tnode_%d [label = \" { type: OPER | val: %s | ptr: %p | par: %p | {<left> %p | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n",  node, Oper_info_arr[ D_OPER(node) ].name,  node, PARENT(node), LEFT(node), node_rank);
        } else {
            PRINTF_LOG("\tnode_%d [label = \" { type: OPER | val: %s | ptr: %p | par: %p | {<left> %p | <right> %p}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n", node, Oper_info_arr[ D_OPER(node) ].name,  node, PARENT(node), LEFT(node), RIGHT(node), node_rank);
        }
        break;
    
    case TYPE_NUM:
        if ( Is_Leaf_Node( (TreeNode_t*) node) == IS_LEAF ) {
            PRINTF_LOG("\tnode_%d [label = \" { type: NUM | val: %lg | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_BLUE ", color = darkblue]\n",   node, D_NUM(node), node, PARENT(node), node_rank);
        } else {
            PRINTF_LOG("\tnode_%d [label = \" { type: NUM | val: %lg | ptr: %p | par: %p | {<left> %p | <right> %p}}\", rank = %d, fillcolor = " BRIGHT_BLUE ", color = darkblue]\n", node, D_NUM(node), node, PARENT(node), LEFT(node), RIGHT(node), node_rank);
        }
        break;
    
    case TYPE_VAR:
        if ( Is_Leaf_Node( (TreeNode_t*) node) == IS_LEAF ) {
            
            PRINTF_LOG("\tnode_%d [label = \" { type: VAR | val: %lg ('%s') | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " PASTEL_PINK ", color = " DARK_PINK "]\n",   node, D_VAR_VAL(node), D_VAR_NAME(node), node, PARENT(node), node_rank);
        } else {
            PRINTF_LOG("\tnode_%d [label = \" { type: VAR | val: %lg ('%s') | ptr: %p | par: %p | {<left> %p | <right> %p}}\", rank = %d, fillcolor = " PASTEL_PINK ", color = " DARK_PINK "]\n", node, D_VAR_VAL(node), D_VAR_NAME(node), node, LEFT(node), RIGHT(node), PARENT(node), node_rank);
        }
        break;

    default:
        break;
    }

    if (node->left) {
        PRINTF_LOG("\tnode_%d: <left> -> node_%d [dir = both]\n", node, node->left);
    } if (node->right) {
        PRINTF_LOG("\tnode_%d: <right> -> node_%d [dir = both]\n", node, node->right);
    }
    
    --node_rank;
    return END_WITH_SUC;
}

TreeErr_t Dump_Node_to_HTML(const TreeNode_t* node, const char* image_file_name, const char* dump_info) {
    assert(node);

    fprintf(Logfile, "<pre>\n");
    fprintf(Logfile, "\t<h2> Dump of tree (%s): </h2>\n", dump_info);
    fprintf(Logfile, "\t<h2> <span style = \"color: blue;\"> No critical errors </span></h2>\n");

    fprintf(Logfile, "\n<h3>Image:</h3>\n");
    fprintf(Logfile, "\t<img src = %s width = \"600\" height = \"600\">\n\n", image_file_name);

    fprintf(Logfile, "</pre>");

    return END_WITH_SUC;
}

TreeErr_t Dump_disk_buffer_html (char* disk_buffer, size_t cur_pos, size_t size_of_file) {
    assert(disk_buffer);

    fprintf(Logfile, "<h1>Dump of disk_buffer</h1>\n");

    fprintf(Logfile, "<span style = \"color: grey;\">");

    for (size_t pos = 0; pos < cur_pos; pos++)
        fprintf(Logfile, "%c", disk_buffer[pos]);

    fprintf(Logfile, "</span>");

    fprintf(Logfile, "<span style = \"color: orange;\">%d</span>", disk_buffer[cur_pos]);

    fprintf(Logfile, "<span style = \"color: blue;\">");
    
    for (size_t pos = cur_pos + 1; pos < size_of_file; pos++)
        fprintf(Logfile, "%c", disk_buffer[pos]);

    fprintf(Logfile, "</span>");

    fprintf(Logfile, "</h1>\n");

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
    else                          fprintf(stdout, T_BG_YELLOW "%c" T_BG_RESET, disk_buffer[cur_pos]);
    
    for (size_t pos = cur_pos + 1; pos < size_of_file; pos++) {
        if (disk_buffer[pos] == '\0') fprintf(stdout, T_RED "0" T_RESET);
        else                          fprintf(stdout, T_BRIGHT_BLUE "%c" T_RESET, disk_buffer[pos]);
    }

    fprintf(stdout, "\"\n");

    return END_WITH_SUC;
}

//----------------------------------------------------------------------------------------------------------------------

#undef PRINTF_LOG