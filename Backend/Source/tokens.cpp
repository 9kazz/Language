#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tokens.h"

#define FILL_ONE_TOKEN(str, code) { str, code, #code, strlen(str) }

Token_info Token_Info_Arr[] =
{
    FILL_ONE_TOKEN ( "func",   _FUNC_INIT_),
    FILL_ONE_TOKEN ( "var",    _VAR_INIT_),
    FILL_ONE_TOKEN ( "(",      _OPEN_BRACK_),
    FILL_ONE_TOKEN ( ")",      _CLOSE_BRACK_),
    FILL_ONE_TOKEN ( "{",      _BEGIN_OPER_),
    FILL_ONE_TOKEN ( "}",      _END_OPER_),
    FILL_ONE_TOKEN ( ";",      _END_STATEMENT_),
    FILL_ONE_TOKEN ( ",",      _COMMA_),
    FILL_ONE_TOKEN ( "==",     _LOG_EQUAL_),
    FILL_ONE_TOKEN ( "more",   _LOG_MORE_),
    FILL_ONE_TOKEN ( "less",   _LOG_LESS_),
    FILL_ONE_TOKEN ( "=",      _ASSIGNMENT_),
    FILL_ONE_TOKEN ( "return", _RETURN_),
    FILL_ONE_TOKEN ( "print" , _PRINT_),
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

size_t TOKEN_COUNT = sizeof(Token_Info_Arr) / sizeof(Token_Info_Arr[0]);


#define FILL_ONE_TYPE(name) { #name, TYPE_##name}

Type_Info Type_Arr[] =
{
    FILL_ONE_TYPE(OPER),
    FILL_ONE_TYPE(NUM),
    FILL_ONE_TYPE(VAR),
    FILL_ONE_TYPE(FUNC),
    FILL_ONE_TYPE(VAR_INIT),
    FILL_ONE_TYPE(FUNC_INIT)
};

#undef FILL_ONE_TYPE

size_t TYPES_COUNT = sizeof(Type_Arr) / sizeof(Type_Arr[0]);

//----------------------------------------------------------------------------------------------

Token_t Find_Oper(char* oper_name) {
    assert(oper_name);

    for (size_t oper_idx = 0; oper_idx < TOKEN_COUNT; oper_idx++) 
    {
        if (strcmp(Token_Info_Arr[oper_idx].enum_name, oper_name) == 0)
            return Token_Info_Arr[oper_idx].code;
    }

    return _UNDEF_TOKEN_;
}