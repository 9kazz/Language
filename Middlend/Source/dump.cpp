#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "verify.h"
#include "dump.h"
#include "types.h"
#include "operators.h"

//----------------------------------------------------------------------------------------------------------------------

#define PRINTF_OUTPUT(format, ...)  fprintf(output_file, format, ##__VA_ARGS__);
#define PRINTF_LOG(format, ...)     fprintf(Logfile, format, ##__VA_ARGS__);

//----------------------------------------------------------------------------------------------------------------------


TreeErr_t Graphic_Dump_Node(const Tree_t* tree, const char* dump_info) {
    assert(tree);

    static size_t dump_num = 1;

    static const size_t    MAX_STR_LEN  = 256;
    char image_name_str   [MAX_STR_LEN] = {0};
    char create_image_CMD [MAX_STR_LEN] = {0};

    SAFE_SNPRINTF(image_name_str, MAX_STR_LEN, "dump_files/image_%d.png", dump_num); 

    SAFE_FOPEN(graphviz_file, "dump_files/graphviz.txt", "w")

    Print_Tree_to_Graphviz(tree, graphviz_file);

    fclose(graphviz_file);

    SAFE_SNPRINTF(create_image_CMD, MAX_STR_LEN, "dot -Tpng dump_files/graphviz.txt -o %s", image_name_str);

    system(create_image_CMD);

    Dump_Node_to_HTML(tree->root, image_name_str, dump_info);

    ++dump_num;

    return END_WITH_SUC;
}

TreeErr_t Print_Tree_to_Graphviz(const Tree_t* tree, FILE* output_file) {
    assert(tree);
    assert(output_file);

    fprintf(output_file, "digraph Tree {\n");
    fprintf(output_file, "\trankdir = TB;\n");
    fprintf(output_file, "\tnode [" SHAPE STYLE " fillcolor = " BRIGHT_GREEN ", color = darkgreen, width = 1, height = 1];\n");
    fprintf(output_file, "\tedge [color = \"darkgreen\"];\n\n");

    Print_Node_to_Graphviz(tree->root, output_file);

    fprintf(output_file, "\n}\n");

    return END_WITH_SUC;
}

TreeErr_t Print_Node_to_Graphviz(const TreeNode_t* node, FILE* output_file) {
    assert(node);
    assert(output_file);

    static int node_rank = 0;
    ++node_rank;

    if (node->left){
        Print_Node_to_Graphviz(node->left, output_file);
    } if (node->right) {
        Print_Node_to_Graphviz(node->right, output_file);
    }

    // if ( Is_Leaf_Node( (TreeNode_t*) node) == IS_LEAF ) {
    //     fprintf(output_file, "\tnode_%d [label = \" { %s | anc = %p | ptr = %p | {<left> 0 | <right> 0}}\", rank = %d]\n", node, node->data, node->parent, node, node_rank);
    // } else if (node->right == NULL && node->left != NULL) {
    //     fprintf(output_file, "\tnode_%d [label = \" { %s | anc = %p | ptr = %p | {<left> %p | <right> 0}}\", rank = %d]\n", node, node->data, node->parent, node, node->left, node_rank);
    // } else {
    //     fprintf(output_file, "\tnode_%d [label = \" { %s | anc = %p | ptr = %p | {<left> %p | <right> %p}}\", rank = %d]\n", node, node->data, node->parent, node, node->left, node->right, node_rank);
    // }

    switch ( TYPE(node) )
    {
    case TYPE_OPER:
        if ( Is_Leaf_Node( (TreeNode_t*) node) == IS_LEAF ) {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: OPER | key word: '%s' | code: %s (%d) | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n",   node, Token_Info_Arr[node->data.oper - 1].key_word, Token_Info_Arr[node->data.oper - 1].enum_name, Token_Info_Arr[node->data.oper - 1].code, node, PARENT(node), node_rank);
        } else if ( RIGHT(node) == NULL && LEFT(node) != NULL) {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: OPER | key word: '%s' | code: %s (%d) | ptr: %p | par: %p | {<left> %p | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n",  node, Token_Info_Arr[node->data.oper - 1].key_word, Token_Info_Arr[node->data.oper - 1].enum_name, Token_Info_Arr[node->data.oper - 1].code, node, PARENT(node), LEFT(node), node_rank);
        } else {
            PRINTF_OUTPUT("\tnode_%d [label = \" { type: OPER | key word: '%s' | code: %s (%d) | ptr: %p | par: %p | {<left> %p | <right> %p}}\", rank = %d, fillcolor = " BRIGHT_GREEN ", color = darkgreen]\n", node, Token_Info_Arr[node->data.oper - 1].key_word, Token_Info_Arr[node->data.oper - 1].enum_name, Token_Info_Arr[node->data.oper - 1].code, node, PARENT(node), LEFT(node), RIGHT(node), node_rank);
        }
        break;
    
    case TYPE_NUM:
        PRINTF_OUTPUT("\tnode_%d [label = \" { type: NUM | val: %lg | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " BRIGHT_BLUE ", color = darkblue]\n",   node, node->data.num, node, PARENT(node), node_rank);
        break;
    
    case TYPE_VAR:
        PRINTF_OUTPUT("\tnode_%d [label = \" { type: VAR | name: '%s' | ptr: %p | par: %p | {<left> 0 | <right> 0}}\", rank = %d, fillcolor = " PASTEL_PINK ", color = " DARK_PINK "]\n",   node, node->data.identifier, node, PARENT(node), node_rank);
        break;

    default:
        break;
    }

    if (node->left) {
        fprintf(output_file, "\tnode_%d: <left> -> node_%d [dir = both]\n", node, node->left);
    } if (node->right) {
        fprintf(output_file, "\tnode_%d: <right> -> node_%d [dir = both]\n", node, node->right);
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

TreeErr_t Dump_disk_buffer (char* disk_buffer, size_t cur_pos, size_t size_of_file) {
    assert(disk_buffer);

    fprintf(stdout, "\"");

    for (size_t pos = 0; pos < cur_pos; pos++) {
        if (disk_buffer[pos] == '\0') fprintf(stdout, T_RED "0" T_RESET);
        else                          fprintf(stdout, T_GREY "%c" T_RESET, disk_buffer[pos]);
    }

    if (disk_buffer[cur_pos] == '\0') fprintf(stdout, T_RED "0" T_RESET);
    else                              fprintf(stdout, T_YELLOW "%c" T_RESET, disk_buffer[cur_pos]);
    
    for (size_t pos = cur_pos + 1; pos < size_of_file; pos++) {
        if (disk_buffer[pos] == '\0') fprintf(stdout, T_RED "0" T_RESET);
        else                          fprintf(stdout, T_BRIGHT_BLUE "%c" T_RESET, disk_buffer[pos]);
    }

    fprintf(stdout, "\"\n");

    return END_WITH_SUC;
}

TreeErr_t Dump_tree_into_file (TreeNode_t* tree_root, const char* output_file) {
    assert(tree_root);
    assert(output_file);

    SAFE_FOPEN(tree_file, output_file, "w");

    Dump_Node_preorder(tree_root, tree_file);

    fclose(tree_file);
    return END_WITH_SUC;
}

TreeErr_t Dump_Node_preorder(const TreeNode_t* node, FILE* output_file) {
    assert(node);
    assert(output_file);

    const size_t MAX_DUMP_LEN     = 256;
    char Result_str[MAX_DUMP_LEN] = {0};

    static size_t offset = 0;
    size_t start_offset  = offset;

    PRINTF_OUTPUT("(");
    offset += strlen("(");

    switch ( TYPE(node) )
    {
    case TYPE_OPER:
        sprintf(Result_str, "\"OPER %s\"", Token_Info_Arr[node->data.oper - 1].enum_name);
        break;
    
    case TYPE_NUM:
        sprintf(Result_str, "\"NUM %lg\"", node->data.num);
        break;

    case TYPE_VAR: 
        sprintf(Result_str, "\"VAR %s\"", node->data.identifier);
        break;

    case TYPE_FUNC:
        sprintf(Result_str, "\"FUNC %s\"", node->data.identifier);
        break;
        
    default:
        break;
    }

    PRINTF_OUTPUT("%s", Result_str);
    offset += strlen(Result_str);

    if (node->left) {
        Dump_Node_preorder(node->left, output_file);
    } else {
        PRINTF_OUTPUT(" nil");
    }

    if (node->right) {
        PRINTF_OUTPUT("\n");
        Print_Space(offset, output_file);
        Dump_Node_preorder(node->right, output_file);
    } else {
        PRINTF_OUTPUT("\n");
        Print_Space(offset, output_file);
        PRINTF_OUTPUT(" nil");
    }

    PRINTF_OUTPUT(")");
    offset += strlen(")");

    offset = start_offset;
    return END_WITH_SUC;
}   

TreeErr_t Print_Space(size_t spaces_count, FILE* output_file) {
    assert(output_file);
    
    for (size_t cur_count = 0; cur_count < spaces_count; cur_count++)
        PRINTF_OUTPUT(" ");

    return END_WITH_SUC;
}

//----------------------------------------------------------------------------------------------------------------------
#define PRINTF_OUTPUT
#define PRINTF_LOG
