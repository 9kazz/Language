#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "stack.h"
#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "lexical.h"
#include "verify.h"
#include "tree.h"

//-----------------------------------------------------------------------------------

#define FILL_ONE_TOKEN(str, code) { str, code, strlen(str) }

static Token_info Token_Info_Arr[] =
    {
        FILL_ONE_TOKEN ( "(",      _OPEN_BRACK_),
        FILL_ONE_TOKEN ( ")",      _CLOSE_BRACK_),
        FILL_ONE_TOKEN ( "==",     _LOG_EQUAL_),
        FILL_ONE_TOKEN ( "=",      _ASSIGNMENT_),
        FILL_ONE_TOKEN ( "if",     _IF_),
        FILL_ONE_TOKEN ( "else",   _ELSE_),
        FILL_ONE_TOKEN ( "while",  _WHILE_),
        FILL_ONE_TOKEN ( "$",      _END_)
    };

#undef FILL_ONE_TOKEN

static size_t TOKEN_COUNT = sizeof(Token_Info_Arr) / sizeof(Token_Info_Arr[0]);

//-----------------------------------------------------------------------------------

Tree_t* Create_Tree_from_disk(const char* input_file_name) {
    assert(input_file_name);

    SAFE_FOPEN(disk_file, input_file_name, "r");

    Disk_t* disk_buf = Read_file_to_buffer(disk_file);
    char*   str      = disk_buf->buffer;

    SAFE_CALLOC(tree, 1, Tree_t);

    DISK(tree) = disk_buf;

    return tree;
}

void Test_Grammar(const char* input_file_name) {
    assert(input_file_name);

    SAFE_FOPEN(disk_file, input_file_name, "r");

    Disk_t* disk_buf = Read_file_to_buffer(disk_file);
    char*   str      = disk_buf->buffer;


    Stack_str* token_stk = Stack_Ctor(10);
    
    for (size_t idx = 0; idx < STK_CAPACITY(token_stk); idx++)
        STK_DATA(token_stk)[idx].code = _UNDEF_TOKEN_;

    size_t count_of_tokens = Fill_token_array(str, STK_DATA(token_stk));

    printf("count_of_tokens: %d\n", count_of_tokens);

    for(size_t idx = 0; idx < STK_CAPACITY(token_stk); idx++)
        printf("%d\t%s\n", STK_DATA(token_stk)[idx].code, STK_DATA(token_stk)[idx].name);

    Stack_Dtor(token_stk);

    // SAFE_CALLOC(token_arr, 10, Token_str);
    // for (size_t i = 0; i < 10; i++) {
    //     token_arr[i].code = _UNDEF_TOKEN_;
    // }
    // size_t count_of_tokens = Fill_token_array(str, token_arr);

    // printf("count_of_tokens: %d\n", count_of_tokens);
    // for(size_t i = 0; i < TOKEN_COUNT; i++) {
    //     printf("%d\t%s\n", token_arr[i].code, token_arr[i].name);
    // }

    // for(size_t i = 0; i < TOKEN_COUNT; i++) {
    //     if (token_arr[i].name != NULL)  free(token_arr[i].name);
    // }
    // free(token_arr);

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

size_t Fill_token_array(char* str, Token_str* token_arr) {
    assert(str);
    assert(token_arr);

    Token_str* token_ptr_at_start = token_arr;

    while (*str != '\0') 
    {   
        skip_space(&str);

        for (size_t cur_tok = 0; cur_tok < TOKEN_COUNT; cur_tok++)
        {
            if ( strncmp(str, Token_Info_Arr[cur_tok].key_word, Token_Info_Arr[cur_tok].len) == 0 )
            {
                token_arr->code = Token_Info_Arr[cur_tok].code;
                str += Token_Info_Arr[cur_tok].len;
                break;
            }
        }

        if (token_arr->code != _UNDEF_TOKEN_)
        {
            token_arr++ ;
            continue;
        }

        const size_t MAX_ID_LEN = 64;
        SAFE_CALLOC(id_name, MAX_ID_LEN, char)

        for (size_t cur_id_len = 0; 
            (isalpha(*str) || isdigit(*str) || *str == '_')  &&  cur_id_len < MAX_ID_LEN; 
            cur_id_len++)
        {
            id_name[cur_id_len] = *str;
            str++ ;
        }

        // size_t id_len = 0;
        // while (isalpha(*str) || isdigit(*str) || *str == '_') 
        // {
        //     id_len++ ;
        // }

        // char* id_name = strndup(str, id_len);


        if (*id_name == '\0')
        {
            fprintf(stderr, "%s: syntax error (%s:%d)\n", __func__, __FILE__, __LINE__);
            return 0;
        }

        token_arr->name = id_name;
        token_arr++ ;
    }

    return token_arr - token_ptr_at_start; ;
}

