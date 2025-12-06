#ifndef DISK_H
#define DISK_H

#include <stdio.h>

#include "types.h"

Tree_t*     Create_Tree_from_disk (const char* input_file_name);

Disk_t*     Read_file_to_buffer   (FILE* input_file);
Stack_str*  Create_token_stack    (char* str); 

size_t      Fill_token_stack    (char* str, Stack_str* token_stk);

#endif