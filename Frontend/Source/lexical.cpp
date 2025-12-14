#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "Tokens.h"
#include "lexical.h"
#include "stack.h"
#include "types.h"
#include "Logfiles.h"
#include "utils.h"
#include "verify.h"
#include "tree.h"
#include "grammar.h"

//-----------------------------------------------------------------------------------

Tree_t* Create_Tree_from_disk(const char* input_file_name) {
    assert(input_file_name);

    SAFE_FOPEN(disk_file, input_file_name, "r");

    Disk_t* disk_buf = Read_file_to_buffer(disk_file);
    char*   str      = disk_buf->buffer;

    Stack_str* token_stk = Create_token_stack(str);
    
    SAFE_CALLOC(tree, 1, Tree_t);

    TREE_TKN(tree) = token_stk;

    ONDEBUG(printf("String\tCode\tName\tValue\t\tLine\tPos\n");
            for(size_t idx = 0; idx < STK_CAPACITY(token_stk); idx++) {
                if (STK_DATA(token_stk)[idx].code > 0) {printf("%s\t", Token_Info_Arr[STK_DATA(token_stk)[idx].code - 1].key_word);}
                else {printf("%s\t", STK_DATA(token_stk)[idx].name);}
                printf("%d\t%s\t%lg\t", STK_DATA(token_stk)[idx].code, STK_DATA(token_stk)[idx].name, STK_DATA(token_stk)[idx].value);
                printf("\t%d\t%d\n", STK_DATA(token_stk)[idx].lex_info.line, STK_DATA(token_stk)[idx].lex_info.pos);
            } 
    );
    
    ROOT(tree) = Get_Grammar( &STK_DATA(token_stk) );
    
    ONDEBUG(printf("\nString\tCode\tName\tValue\t\tLine\tPos\n");
            for(size_t idx = 0; idx < STK_CAPACITY(token_stk); idx++) {
                if (STK_DATA(token_stk)[idx].code > 0) {printf("%s\t", Token_Info_Arr[STK_DATA(token_stk)[idx].code - 1].key_word);}
                else {printf("%s\t", STK_DATA(token_stk)[idx].name);}
                printf("%d\t%s\t%lg\t", STK_DATA(token_stk)[idx].code, STK_DATA(token_stk)[idx].name, STK_DATA(token_stk)[idx].value);
                printf("\t%d\t%d\n", STK_DATA(token_stk)[idx].lex_info.line, STK_DATA(token_stk)[idx].lex_info.pos);
            } 
    );

    free(str);
    free(disk_buf);
    
    return tree;
}

Disk_t* Read_file_to_buffer(FILE* input_file) {
    assert(input_file);

    SAFE_CALLOC(disk, 1, Disk_t)

    struct stat file_info;

    int file_descriptor = fileno(input_file);

    SAFE_FSTAT(file_descriptor, &file_info)

    size_t size_of_file = file_info.st_size;

    SAFE_CALLOC(tree_buf, size_of_file + 2, char)    

    SAFE_FREAD(tree_buf, sizeof(char), size_of_file, input_file)

    tree_buf[size_of_file]     = '$';
    tree_buf[size_of_file + 1] = '\0';
    
    disk->buffer = tree_buf;
    disk->size   = size_of_file;

    return disk;
}

Stack_str* Create_token_stack (char* str) {
    assert(str);

    const size_t START_STACK_CAPACITY = 5;

    Stack_str* token_stk = Stack_Ctor( START_STACK_CAPACITY );

    size_t count_of_tokens = Fill_token_stack(str, token_stk);

    return token_stk;
}

#define NEXT_TOKEN                                            \
    if (STK_SIZE(token_stk) >= STK_CAPACITY(token_stk) - 1)   \
        Stack_Realloc(token_stk);                             \
    STK_SIZE(token_stk)++ ;                                   \
    token_arr = STK_DATA(token_stk) + STK_SIZE(token_stk);

size_t Fill_token_stack (char* str, Stack_str* token_stk) {
    assert(str);
    assert(token_stk);

    char* str_at_start   = str;
    Token_str* token_arr = STK_DATA(token_stk);

    size_t line_num = 1;

    while (*str != '\0') 
    {   
        line_num += skip_space(&str);

        for (size_t cur_tok = 0; cur_tok < TOKEN_COUNT; cur_tok++)
        {
            if ( strncmp(str, Token_Info_Arr[cur_tok].key_word, Token_Info_Arr[cur_tok].len) == 0 )
            {
                TKN_CODE(token_arr)      = Token_Info_Arr[cur_tok].code;
                token_arr->lex_info.pos  = str - str_at_start;
                token_arr->lex_info.line = line_num;
                
                str += Token_Info_Arr[cur_tok].len;
                break;
            }
        }

        if (TKN_CODE(token_arr) != _UNDEF_TOKEN_)
        {
            NEXT_TOKEN;
            continue;
        }

        size_t id_len = 0;
        while (isalpha(*(str + id_len)) || isdigit(*(str + id_len)) || *(str + id_len) == '_')
        {
            id_len++ ;
        }

        char* id_name = strndup(str, id_len);
        str += id_len;

        if (*id_name == '\0')
        {
            fprintf(stderr, "%s: syntax error (%s:%d)\n", __func__, __FILE__, __LINE__);
            return 0;
        }

        TKN_NAME(token_arr)      = id_name;
        token_arr->lex_info.pos  = str - str_at_start;
        token_arr->lex_info.line = line_num;
        
        NEXT_TOKEN;
    }

    return token_arr - STK_DATA(token_stk);
}

#undef NEXT_TOKEN