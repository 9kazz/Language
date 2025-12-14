#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "operators.h"

#define FILL_ONE_OPER(code) {code, #code}

Oper_info Oper_Info_Arr[] =
{
    FILL_ONE_OPER (_OPEN_BRACK_),
    FILL_ONE_OPER (_CLOSE_BRACK_),
    FILL_ONE_OPER (_BEGIN_OPER_),
    FILL_ONE_OPER (_END_OPER_),
    FILL_ONE_OPER (_END_STATEMENT_),
    FILL_ONE_OPER (_LOG_EQUAL_),
    FILL_ONE_OPER (_ASSIGNMENT_),
    FILL_ONE_OPER (_IF_),
    FILL_ONE_OPER (_ELSE_),
    FILL_ONE_OPER (_WHILE_),
    FILL_ONE_OPER (_END_PROGRAM_),
    
    FILL_ONE_OPER (_MATH_ADD_),
    FILL_ONE_OPER (_MATH_SUB_),
    FILL_ONE_OPER (_MATH_MUL_),
    FILL_ONE_OPER (_MATH_DIV_),
    FILL_ONE_OPER (_MATH_POW_),
    FILL_ONE_OPER (_MATH_SQRT_),
    FILL_ONE_OPER (_MATH_SIN_),
    FILL_ONE_OPER (_MATH_COS_),
    FILL_ONE_OPER (_MATH_TAN_),
    FILL_ONE_OPER (_MATH_COT_),
    FILL_ONE_OPER (_MATH_ASIN_),
    FILL_ONE_OPER (_MATH_ACOS_),
    FILL_ONE_OPER (_MATH_ATAN_),
    FILL_ONE_OPER (_MATH_ACOT_),
    FILL_ONE_OPER (_MATH_SINH_),
    FILL_ONE_OPER (_MATH_COSH_),
    FILL_ONE_OPER (_MATH_TANH_),
    FILL_ONE_OPER (_MATH_COTH_),
    FILL_ONE_OPER (_MATH_EXP_),
    FILL_ONE_OPER (_MATH_LN_)
};

#undef FILL_ONE_OPER

size_t OPER_COUNT = sizeof(Oper_Info_Arr) / sizeof(Oper_Info_Arr[0]);


Type_Info Type_Arr[] =
{
    { "OPER", TYPE_OPER },
    { "NUM",  TYPE_NUM  },
    { "VAR",  TYPE_VAR  },
    { "FUNC", TYPE_FUNC }
};

size_t TYPES_COUNT = sizeof(Type_Arr) / sizeof(Type_Arr[0]);

//----------------------------------------------------------------------------------------------

Operators Find_Oper(char* oper_name) {
    assert(oper_name);

    for (size_t oper_idx = 0; oper_idx < OPER_COUNT; oper_idx++) 
    {
        if (strcmp(Oper_Info_Arr[oper_idx].enum_name, oper_name) == 0)
            return Oper_Info_Arr[oper_idx].code;
    }

    return _UNDEF_TOKEN_;
}