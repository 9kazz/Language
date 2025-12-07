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
#include "grammar.h"

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
        FILL_ONE_TOKEN ( "$",      _END_PROGRAM_),
        
        FILL_ONE_TOKEN ( "+",      _MATH_ADD_),
        FILL_ONE_TOKEN ( "-",      _MATH_SUB_),
        FILL_ONE_TOKEN ( "*",      _MATH_MUL_),
        FILL_ONE_TOKEN ( "/",      _MATH_DIV_),
        FILL_ONE_TOKEN ( "^",      _MATH_POW_),
        FILL_ONE_TOKEN ( "sqrt",   _MATH_SQRT_),
        FILL_ONE_TOKEN ( "sin",    _MATH_SIN_),
        FILL_ONE_TOKEN ( "cos",    _MATH_COS_),
        FILL_ONE_TOKEN ( "tan",    _MATH_TAN_),
        FILL_ONE_TOKEN ( "cot",    _MATH_COT_),
        FILL_ONE_TOKEN ( "asin",   _MATH_ASIN_),
        FILL_ONE_TOKEN ( "acos",   _MATH_ACOS_),
        FILL_ONE_TOKEN ( "atan",   _MATH_ATAN_),
        FILL_ONE_TOKEN ( "acot",   _MATH_ACOT_),
        FILL_ONE_TOKEN ( "sinh",   _MATH_SINH_),
        FILL_ONE_TOKEN ( "cosh",   _MATH_COSH_),
        FILL_ONE_TOKEN ( "tanh",   _MATH_TANH_),
        FILL_ONE_TOKEN ( "coth",   _MATH_COTH_),
        FILL_ONE_TOKEN ( "exp",    _MATH_EXP_),
        FILL_ONE_TOKEN ( "ln",     _MATH_LN_)
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

    TREE_TKN(tree) = token_stk;

    ONDEBUG(printf("String\tCode\tName\tValue\t\tLine\tPos\n");
            for(size_t idx = 0; idx < STK_CAPACITY(token_stk); idx++) {
                if (STK_DATA(token_stk)[idx].code > 0) {printf("%s\t", Token_Info_Arr[STK_DATA(token_stk)[idx].code - 1].key_word);}
                else {printf("%s\t", STK_DATA(token_stk)[idx].name);}
                printf("%d\t%s\t%lg\t", STK_DATA(token_stk)[idx].code, STK_DATA(token_stk)[idx].name, STK_DATA(token_stk)[idx].value);
                printf("\t%d\t%d\n", STK_DATA(token_stk)[idx].lex_info.line, STK_DATA(token_stk)[idx].lex_info.pos);
            } 
    );
    
    ROOT(tree)     = Get_Grammar( &STK_DATA(token_stk) );
    
    ONDEBUG(printf("String\tCode\tName\tValue\t\tLine\tPos\n");
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

    const size_t START_STACK_CAPACITY = 20;

    Stack_str* token_stk = Stack_Ctor( START_STACK_CAPACITY );

    size_t count_of_tokens = Fill_token_stack(str, token_stk);

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

