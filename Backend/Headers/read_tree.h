#ifndef DISK_H
#define DISK_H

#include <stdio.h>

#include "utils.h"
#include "tree.h"

Tree_t*     Create_Tree_from_disk     (const char* input_file_name);

TreeNode_t* Create_Node_from_disk     (size_t* cur_pos, DiskBuf_t* buffer);
DiskBuf_t*  Create_tree_buf_from_disk (FILE* input_file);

Data_Type   Identify_Type             (char* type_str);
NodeData_t  Identify_Data             (char* data_str, Data_Type type);

char*       Read_Node_Name_from_disk  (size_t* cur_pos, DiskBuf_t* buffer);
char*       Read_Node_Type_from_disk  (size_t* cur_pos, DiskBuf_t* disk_buf);



int         Is_nil (size_t cur_pos, DiskBuf_t* buffer);

#endif