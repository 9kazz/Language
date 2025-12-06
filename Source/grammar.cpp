#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "types.h"
#include "Logfile.h"
#include "utils.h"
#include "lexical.h"
#include "verify.h"
#include "tree.h"

//---------------------------------------------------------------------------------------------------------------------

#define CTOR_ADD(left_son, right_son)  Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_ADD},  left_son, right_son)
#define CTOR_SUB(left_son, right_son)  Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_SUB},  left_son, right_son)
#define CTOR_MUL(left_son, right_son)  Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_MUL},  left_son, right_son)
#define CTOR_DIV(left_son, right_son)  Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_DIV},  left_son, right_son)
#define CTOR_POW(left_son, right_son)  Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_POW},  left_son, right_son)
#define CTOR_SQRT(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_SQRT}, left_son, NULL)

#define CTOR_SIN(left_son)             Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_SIN},  left_son, NULL)
#define CTOR_COS(left_son)             Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_COS},  left_son, NULL)
#define CTOR_TAN(left_son)             Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_TAN},  left_son, NULL)
#define CTOR_COT(left_son)             Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_COT},  left_son, NULL)

#define CTOR_ASIN(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_ASIN}, left_son, NULL)
#define CTOR_ACOS(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_ACOS}, left_son, NULL)
#define CTOR_ATAN(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_ATAN}, left_son, NULL)
#define CTOR_ACOT(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_ACOT}, left_son, NULL)

#define CTOR_SINH(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_SINH}, left_son, NULL)
#define CTOR_COSH(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_COSH}, left_son, NULL)
#define CTOR_TANH(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_TANH}, left_son, NULL)
#define CTOR_COTH(left_son)            Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_COTH}, left_son, NULL)

#define CTOR_EXP(left_son)             Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_EXP},  left_son, NULL)
#define CTOR_LN(left_son)              Node_Ctor(TYPE_OPER, (NodeData_t) {.oper = OP_LN},   left_son, NULL)    

#define CTOR_CONST(number)             Node_Ctor(TYPE_NUM,  (NodeData_t) {.num = number},   NULL,     NULL)
#define CTOR_VAR(var_name)             Node_Ctor(TYPE_VAR,  (NodeData_t) {.var = var_name}, NULL,     NULL)

//---------------------------------------------------------------------------------------------------------------------

#define TKN_CODE(token)  (token)->code
#define TKN_NAME(token)  (token)->name

#define NEXT_TOKEN       (*token)++ ;

//---------------------------------------------------------------------------------------------------------------------

TreeNode_t* Get_Grammar(Token_str** token) {    
    assert( token); 
    assert(*token); 

    TreeNode_t* result = Get_Statement(token); 
    
    if ( TKN_CODE(*token) != _END_PROGRAM_) 
    {
        fprintf(stderr, "%s in %s:%d: Sintax error\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    NEXT_TOKEN;
    return result;
}

TreeNode_t* Get_Statement(Token_str** token) {    
    assert( token); 
    assert(*token); 

    TreeNode_t* result = Get_Operator(token); 
    
    if ( TKN_CODE(*token) != _END_STATEMENT_) 
    {
        fprintf(stderr, "%s in %s:%d: Sintax error\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    NEXT_TOKEN;
    return result;
}

TreeNode_t* Get_Operator(Token_str** token) {    
    assert( token); 
    assert(*token); 
    
    TreeNode_t* result = NULL;

    switch ( TKN_CODE(*token) )
    {
    case _ASSIGNMENT_:  result = Get_Assignment (token);  break;
    case _IF_:          result = Get_If_oper    (token);  break;
    case _WHILE_:       result = Get_While_oper (token);  break;

    default:            result = NULL;                    break;
    }

    return result;
}

TreeNode_t* Get_Block(Token_str** token) {    
    assert( token); 
    assert(*token); 
    
    TreeNode_t* result = NULL;

    if ( TKN_CODE(*token) == _BEGIN_OPER_ )
    {
        NEXT_TOKEN;

        do {
            Get_Statement(token);
        } while()
    }

    NEXT_TOKEN;
    return result;
}

TreeNode_t* Get_While_oper(Token_str** token) {
    assert( token);
    assert(*token);

    TreeNode_t* result = NULL;

}

TreeNode_t* Get_If_oper(Token_str** token) {
    assert( token);
    assert(*token);

    TreeNode_t* result = NULL;

}

TreeNode_t* Get_Assignment(Token_str** token) {
    assert( token);
    assert(*token);

    TreeNode_t* left_hand_side  = Get_Variable(token);

    if ( TKN_CODE(*token) != _ASSIGNMENT_) 
    {
        fprintf(stderr, "%s in %s:%d: Sintax error\n", __func__, __FILE__, __LINE__);
        return NULL;
    }

    NEXT_TOKEN;
    
    TreeNode_t* right_hand_side = Get_Equation(token);

}

TreeNode_t* Get_Equation(Token_str** token) {
    assert( token);
    assert(*token);   

    char* str_before = *str;

    TreeNode_t* result    = NULL;
    TreeNode_t* right_son = NULL;
    
    result = Get_Term(str);

    while( TKN_CODE(*token) == _ADD_OP_ || TKN_CODE(*token) == _SUB_OP_ ) 
    {
        int oper = **str;
        NEXT_TOKEN;


        TreeNode_t* right_son = Get_Term(str);


        if (oper == '+')  
            result = CTOR_ADD(result, right_son);
        else
            result = CTOR_SUB(result, right_son);              
    }

    if (str_before == *str) 
    {
        fprintf(stderr, "Get_Term: Sintax error\n");
        return NULL;
    }

    return result;
}

TreeNode_t* Get_Term(char** str) 
{
    assert(str);
    assert(*str);

    ONDEBUG(printf("Ter: %s\n", *str);)

    char* str_before = *str;

    TreeNode_t* result    = NULL;
    TreeNode_t* right_son = NULL;
    
    result = Get_Power(str);

    while(**str == '*' || **str == '/') 
    {
        int oper = **str;
        (*str)++ ;


        right_son = Get_Power(str);


        if (oper == '*')  
            result = CTOR_MUL(result, right_son);
        else
            result = CTOR_DIV(result, right_son);
    }

    if (str_before == *str) 
    {
        fprintf(stderr, "Get_Term: Sintax error\n");
        return NULL;
    }

    return result;
}

TreeNode_t* Get_Power(char** str) 
{
    assert(str);
    assert(*str);

    ONDEBUG(printf("Pow: %s\n", *str);)

    char* str_before = *str;

    TreeNode_t* result    = NULL;
    TreeNode_t* right_son = NULL;
      
    result = Get_Primary(str);

    if (**str == '^') 
    {
        (*str)++ ;


        right_son = Get_Primary(str);


        result = CTOR_POW(result, right_son);
    }

    if (str_before == *str) 
    {
        fprintf(stderr, "Get_Term: Sintax error\n");
        return NULL;
    }

    return result;
}

#define RETURN_WITH_FREE(value_to_return)   \
    result = (value_to_return);             \
    free(token);                            \
    ONDEBUG(printf("Pri out: %s\n", *str);) \
    return result;
    
TreeNode_t* Get_Primary(char** str) 
{
    assert(str);
    assert(*str);

    ONDEBUG(printf("Pri: %s\n", *str);)

    char* str_before = *str;

    TreeNode_t* result = NULL;

    if (**str == '(')
    {
        (*str)++ ;


        result = Get_Equation(str);


        if (**str != ')') 
        {
            fprintf(stderr, "Get_Primary: Sintax error. Not closed brackets\n");
            return NULL;
        }

        (*str)++ ;

        ONDEBUG(printf("Pri out: %s\n", *str);)

        return result;
    }

    const int MAX_TOKEN_LEN = 64;
    SAFE_CALLOC(token, MAX_TOKEN_LEN, char)

    size_t cur_token_len = 0;

    while ( isalpha(**str) || isdigit(**str) ||  **str == '.' ||
            **str == '_'   || **str == '{'   ||  **str == '}'  )
    {
        token[cur_token_len++] = **str;

        if (cur_token_len >= MAX_TOKEN_LEN) 
        {
            ONDEBUG(fprintf(stderr, "Get_Primary: token buffer overflow\n");)
            RETURN_WITH_FREE( NULL );
        }

        (*str)++ ;
    }


    if ( ! isalpha(*token) )
    {
        RETURN_WITH_FREE( Get_Number(str, token) );
    }
    
    if (**str == '(') 
    {
        RETURN_WITH_FREE( Get_Unary(str, token) );
    }

    RETURN_WITH_FREE( Get_Variable(str, token) );
}
#undef RETURN_WITH_FREE

TreeNode_t* Get_Unary(char** str, char* oper_str) 
{
    assert(str);
    assert(*str);
    assert(oper_str);

    ONDEBUG(printf("Una: %s\n", *str); printf("una: [%s]\n", oper_str);)

    Operators   oper_code = OP_UNKNOWN;

    (*str)++ ;

    TreeNode_t* left_son  = Get_Equation(str);

    if (**str != ')') 
    {
        fprintf(stderr, "Get_Unary: Sintax error. Not closed brackets\n");
        return NULL;
    }

    (*str)++ ;

    for (size_t idx = 0; idx < OPER_COUNT; idx++) 
    {
        if (strcmp(oper_str, Oper_info_arr[idx].symbol) == 0)
        {
            oper_code = Oper_info_arr[idx].code;
            break;
        }
    }

    switch (oper_code)
    {
        case OP_UNKNOWN: 
            ONDEBUG(fprintf(stderr, "Get_Unary: unknown oper\n");)
            return NULL; 

        case OP_SQRT:    return CTOR_SQRT(left_son);

        case OP_SIN:     return CTOR_SIN (left_son);
        case OP_COS:     return CTOR_COS (left_son);
        case OP_TAN:     return CTOR_TAN (left_son);
        case OP_COT:     return CTOR_COT (left_son);

        case OP_ASIN:    return CTOR_ASIN(left_son);
        case OP_ACOS:    return CTOR_ACOS(left_son);
        case OP_ATAN:    return CTOR_ATAN(left_son);
        case OP_ACOT:    return CTOR_ACOT(left_son);

        case OP_SINH:    return CTOR_SINH(left_son);
        case OP_COSH:    return CTOR_COSH(left_son);
        case OP_TANH:    return CTOR_TANH(left_son);
        case OP_COTH:    return CTOR_COTH(left_son);

        case OP_EXP:     return CTOR_EXP (left_son);
        case OP_LN:      return CTOR_LN  (left_son);
        
        default:  
            fprintf(stderr, "Get_Unary: operation error\n");
            return NULL;
    }    

    return NULL;
}

TreeNode_t* Get_Variable(Token_str** token) {
    assert( token);
    assert(*token);

    if ( TKN_CODE(*token) != _UNDEF_TOKEN_) {
        fprintf(stderr, "%s in %s:%d: token almost determined with code %d\n", __func__, __FILE__, __LINE__, TKN_CODE(*token));
        return NULL;
    }

    TKN_CODE(*token) = _VARIABLE_;
    char* var_name   = TKN_NAME(*token);

    NEXT_TOKEN;

    return CTOR_VAR(var_name);
}

TreeNode_t* Get_Number(Token_str** token) {
    assert( token);
    assert(*token);

    if ( TKN_CODE(*token) != _UNDEF_TOKEN_) {
        fprintf(stderr, "%s in %s:%d: token almost determined with code %d\n", __func__, __FILE__, __LINE__, TKN_CODE(*token));
        return NULL;
    }

    TKN_CODE(*token) = _NUMBER_;
    double num       = strtod( TKN_NAME(*token), NULL);

    NEXT_TOKEN;

    return CTOR_CONST(num);
}

//---------------------------------------------------------------------------------------------------------------------

#undef CTOR_ADD
#undef CTOR_SUB
#undef CTOR_MUL
#undef CTOR_DIV
#undef CTOR_POW

#undef CTOR_SIN
#undef CTOR_COS
#undef CTOR_TAN
#undef CTOR_COT

#undef CTOR_ASIN
#undef CTOR_ACOS
#undef CTOR_ATAN
#undef CTOR_ACOT

#undef CTOR_SINH
#undef CTOR_COSH
#undef CTOR_TANH
#undef CTOR_COTH

#undef CTOR_LN
#undef CTOR_EXP

#undef CTOR_CONST
#undef CTOR_VAR

//---------------------------------------------------------------------------------------------------------------------

#undef TKN_CODE
#undef TKN_NAME