#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#include "read_tree.h"
#include "verify.h"
#include "dump.h"
#include "types.h"
#include "tokens.h"

Tree_t* Create_Tree_from_disk(const char* input_file_name) {
    assert(input_file_name);

    SAFE_FOPEN(disk_file, input_file_name, "r")

    DiskBuf_t* disk_buf = Create_tree_buf_from_disk(disk_file);

    size_t cur_pos   = 0;
    TreeNode_t* root = Create_Node_from_disk(&cur_pos, disk_buf);
    
    SAFE_CALLOC(tree, 1, Tree_t)

    DISK(tree) = disk_buf;
    ROOT(tree) = root;

    return tree;   
}

TreeNode_t* Create_Node_from_disk(size_t* cur_pos, DiskBuf_t* disk_buf) {
    assert(cur_pos);
    assert(disk_buf);

    const int NIL_LEN = strlen("nil");

    char* buffer     = disk_buf->buffer;
    size_t disk_size = disk_buf->size;

    ONDEBUG( Dump_disk_buffer(disk_buf->buffer, *cur_pos, disk_buf->size); )

    *cur_pos += skip_space(buffer, disk_size, *cur_pos);
    
    if (buffer[*cur_pos] == '(') 
    {
        (*cur_pos)++ ;

        Data_Type  new_node_type = Identify_Type( Read_Node_Type_from_disk(cur_pos, disk_buf) );
        NodeData_t new_node_data = Identify_Data( Read_Node_Name_from_disk(cur_pos, disk_buf), new_node_type); 

        TreeNode_t* new_node = Node_Ctor(new_node_data, new_node_type);

        LEFT(new_node) = Create_Node_from_disk(cur_pos, disk_buf);
        if( LEFT(new_node) ) 
            PARENT( LEFT(new_node) ) = new_node;

        RIGHT(new_node) = Create_Node_from_disk(cur_pos, disk_buf);
        if( RIGHT(new_node) )
            PARENT( RIGHT(new_node) ) = new_node;
        
        (*cur_pos)++ ;

        return new_node;
    }

    else if ( Is_nil(*cur_pos, disk_buf) ) 
    {
        *cur_pos += NIL_LEN;
        *cur_pos += skip_space(buffer, disk_size, *cur_pos);

        return NULL;
    }

    fprintf(stderr, "%s in %s:%d: Unknown symbol in buffer: %c (ANSI: %d) in position: %d\n", __func__, __FILE__, __LINE__, buffer[*cur_pos], buffer[*cur_pos], *cur_pos);
    return NULL;
}

NodeData_t Identify_Data(char* data_str, Data_Type type) {
    assert(data_str);
    assert(type != TYPE_UNKNOWN);

    INIT_ZERO_STRUCT(result, NodeData_t);

    if (type == TYPE_NUM) { 
        result.num = strtod(data_str, NULL);
    
    } else if (type == TYPE_OPER) {
        result.oper = Find_Oper(data_str);

    } else {
        result.identifier = data_str;        
    }

    return result;
}

Data_Type Identify_Type(char* type_str) {
    assert(type_str);

    for (size_t type_idx = 0; type_idx < TYPES_COUNT; type_idx++) 
    {
        if (strcmp(type_str, Type_Arr[type_idx].key_word) == 0)
            return Type_Arr[type_idx].type;
    }

    return TYPE_UNKNOWN;    
}

char* Read_Node_Type_from_disk(size_t* cur_pos, DiskBuf_t* disk_buf) {
    assert(cur_pos);
    assert(disk_buf);
    
    char* buffer     = disk_buf->buffer;
    size_t disk_size = disk_buf->size;

    *cur_pos += skip_space(buffer, disk_size, *cur_pos);
    char* start_of_str = buffer + (*cur_pos);

    size_t node_name_len = 0;

    int sscanf_check = sscanf(start_of_str, " \"%*[^ ]%n", &node_name_len);

    *cur_pos += node_name_len + 1;

    *( buffer + (*cur_pos) - 1) = '\0';

    return start_of_str + 1;
}

char* Read_Node_Name_from_disk(size_t* cur_pos, DiskBuf_t* disk_buf) {
    assert(cur_pos);
    assert(disk_buf);
    
    char* buffer     = disk_buf->buffer;
    size_t disk_size = disk_buf->size;

    *cur_pos += skip_space(buffer, disk_size, *cur_pos);
    char* start_of_str = buffer + (*cur_pos);

    size_t node_name_len = 0;

    int sscanf_check = sscanf(start_of_str, "%*[^\"]\"%n", &node_name_len);

    *cur_pos += node_name_len;

    *( buffer + (*cur_pos) - 1) = '\0';

    return start_of_str;
}

DiskBuf_t* Create_tree_buf_from_disk(FILE* input_file) {
    assert(input_file);

    SAFE_CALLOC(disk, 1, DiskBuf_t)

    struct stat file_info;

    int file_descriptor = fileno(input_file);

    SAFE_FSTAT(file_descriptor, &file_info)

    size_t size_of_file = file_info.st_size;

    SAFE_CALLOC(tree_buf, size_of_file + 1, char)    

    SAFE_FREAD(tree_buf, sizeof(char), size_of_file, input_file)
    tree_buf[size_of_file] = '\0';
    
    disk->buffer = tree_buf;
    disk->size   = size_of_file;

    return disk;
}

int Is_nil(size_t cur_pos, DiskBuf_t* disk_buf) {
    assert(disk_buf);

    char* buffer = disk_buf->buffer;

    if( buffer[cur_pos]   == 'n' &&
        buffer[++cur_pos] == 'i' &&
        buffer[++cur_pos] == 'l' )
    {
        return IS_NIL;
    }

    return NOT_NIL;
}