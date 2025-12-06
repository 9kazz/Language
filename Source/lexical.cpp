#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "lexical.h"
#include "stack.h"
#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "verify.h"
#include "tree.h"

//-----------------------------------------------------------------------------------

#define FILL_ONE_TOKEN(str, code) { str, code, strlen(str) }

static Token_info Token_Info_Arr[] =
    {
        FILL_ONE_TOKEN ( "(",      _OPEN_BRACK_),
        FILL_ONE_TOKEN ( ")",      _CLOSE_BRACK_),
        FILL_ONE_TOKEN ( "{",      _BEGIN_OPER_),
        FILL_ONE_TOKEN ( "}",      _END_OPER_),
        FILL_ONE_TOKEN ( ";",      _END_STATEMENT_),
        FILL_ONE_TOKEN ( "==",     _LOG_EQUAL_),
        FILL_ONE_TOKEN ( "=",      _ASSIGNMENT_),
        FILL_ONE_TOKEN ( "if",     _IF_),
        FILL_ONE_TOKEN ( "else",   _ELSE_),
        FILL_ONE_TOKEN ( "while",  _WHILE_),
        FILL_ONE_TOKEN ( "$",      _END_PROGRAM_)
    };

#undef FILL_ONE_TOKEN

static size_t TOKEN_COUNT = sizeof(Token_Info_Arr) / sizeof(Token_Info_Arr[0]);

//-----------------------------------------------------------------------------------

Tree_t* Create_Tree_from_disk(const char* input_file_name) {
    assert(input_file_name);

    SAFE_FOPEN(disk_file, input_file_name, "r");

    Disk_t* disk_buf = Read_file_to_buffer(disk_file);
    char*   str      = disk_buf->buffer;

    Stack_str* token_stk = Create_token_stack(str);
    
    SAFE_CALLOC(tree, 1, Tree_t);
    DISK(tree) = disk_buf;
    // ROOT(tree) = 
    
    Stack_Dtor(token_stk);
    free(str);
    
    return tree;
}

void Test_Grammar(const char* input_file_name) {
    assert(input_file_name);

    SAFE_FOPEN(disk_file, input_file_name, "r");

    Disk_t* disk_buf = Read_file_to_buffer(disk_file);
    char*   str      = disk_buf->buffer;

    Stack_str* token_stk = Create_token_stack(str);
    Stack_Dtor(token_stk);

    free(str);
    free(disk_buf);
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

    const size_t START_STACK_CAPACITY = 100;

    Stack_str* token_stk = Stack_Ctor( START_STACK_CAPACITY );

    size_t count_of_tokens = Fill_token_stack(str, token_stk);

    ONDEBUG( printf("count_of_tokens: %d\n", count_of_tokens);
             printf("Code\tName\t\tLine\tPos\n");
             for(size_t idx = 0; idx < STK_CAPACITY(token_stk); idx++) {
                printf("%d\t%s\t", STK_DATA(token_stk)[idx].code, STK_DATA(token_stk)[idx].name);
                printf("\t%d\t%d\n", STK_DATA(token_stk)[idx].lex_info.line, STK_DATA(token_stk)[idx].lex_info.pos);
             } 
    );

    return token_stk;
}

#define NEXT_TOKEN          \
    token_arr++ ;           \
    STK_SIZE(token_stk)++ ; \

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
                token_arr->code          = Token_Info_Arr[cur_tok].code;
                token_arr->lex_info.pos  = str - str_at_start;
                token_arr->lex_info.line = line_num;
                
                str += Token_Info_Arr[cur_tok].len;
                break;
            }
        }

        if (token_arr->code != _UNDEF_TOKEN_)
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

        token_arr->name          = id_name;
        token_arr->lex_info.pos  =  str - str_at_start;
        token_arr->lex_info.line = line_num;
        
        NEXT_TOKEN;
    }

    return token_arr - STK_DATA(token_stk);
}

#undef NEXT_TOKEN

