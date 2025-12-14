#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "types.h"
#include "Logfiles.h"
#include "utils.h"
#include "lexical.h"
#include "verify.h"
#include "tree.h"
#include "grammar.h"
#include "Tokens.h"
#include "dump.h"

//---------------------------------------------------------------------------------------------------------------------

#define CTOR_VAR(token)                        Node_Ctor( (token), NULL, NULL)
#define CTOR_NUM(token)                        Node_Ctor( (token), NULL, NULL)
#define CTOR_OPER(token, left_son, right_son)  Node_Ctor( (token), left_son, right_son)

//---------------------------------------------------------------------------------------------------------------------

#define NEXT_TOKEN  (*token)++ ;

#define CHECK_SYNTAX(token_code)                                                                                                                                                                        \
    if ( TKN_CODE(*token) != token_code)                                                                                                                                                                \
    {                                                                                                                                                                                                   \
        fprintf(stderr, "%s in %s:%d:" T_RED " SINTAX ERROR " T_RESET "at line: %d index: %d (expected: %d (%s) | got: %d (%s))\n", __func__, __FILE__, __LINE__, (*token)->lex_info.line, (*token)->lex_info.line, token_code, Token_Info_Arr[token_code - 1].key_word, TKN_CODE(*token), Token_Info_Arr[TKN_CODE(*token) - 1].key_word);   \
        return NULL;                                                                                                                                                                                    \
    }

//---------------------------------------------------------------------------------------------------------------------

TreeNode_t* Get_Grammar(Token_str** token) {    
    assert( token); 
    assert(*token); 
    
    Token_str* token_at_start = *token;
    
    TreeNode_t* result = Get_Statement(token);
    
    if ( ! result)
    {                                                                                   
        fprintf(stderr, "%s in %s:%d: Sintax error\n", __func__, __FILE__, __LINE__);   
        *token = token_at_start;
        return NULL;                                                                    
    }

    TreeNode_t* temp = result;

    while(temp) 
    {
        RIGHT(temp) = Get_Statement(token);
        temp = RIGHT(temp);
    }

    CHECK_SYNTAX(_END_PROGRAM_);
    NEXT_TOKEN;
    
    *token = token_at_start;
    return result;
}

TreeNode_t* Get_Statement(Token_str** token) {    
    assert( token); 
    assert(*token); 
    
    TreeNode_t* statement_body = Get_Operator(token); 
    
    if (statement_body)  {
        CHECK_SYNTAX(_END_STATEMENT_);
        Token_str* result_token = *token;
        NEXT_TOKEN;
        return CTOR_OPER(result_token, statement_body, NULL);
    }
    
    return NULL;
}

TreeNode_t* Get_Operator(Token_str** token) {    
    assert( token); 
    assert(*token); 
    
    if ( TKN_CODE(*token + 1) == _ASSIGNMENT_)
        return Get_Assignment(token);
        
    if ( TKN_CODE(*token) == _IF_)
        return Get_If_oper(token);

    if ( TKN_CODE(*token) == _WHILE_)
        return Get_While_oper(token);
    
    return NULL;
}

TreeNode_t* Get_Block(Token_str** token) {    
    assert( token); 
    assert(*token); 
    
    if ( TKN_CODE(*token) == _BEGIN_OPER_ )
    {
        NEXT_TOKEN;
    
        TreeNode_t* result = Get_Statement(token);
    
        if ( ! result)
        {                                                                                   
            fprintf(stderr, "%s in %s:%d: Sintax error\n", __func__, __FILE__, __LINE__);   
            return NULL;                                                                    
        }

        TreeNode_t* temp = result;

        while(temp) 
        {    
            RIGHT(temp) = Get_Statement(token);    
            temp = RIGHT(temp);
        }

        CHECK_SYNTAX(_END_OPER_);
        NEXT_TOKEN;
    
        return result;
    }

    TreeNode_t* result = Get_Statement(token);

    return result;
}

TreeNode_t* Get_While_oper(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_WHILE_);
    Token_str* result_token = *token;
    NEXT_TOKEN;

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* condition = Get_Equation(token);

    CHECK_SYNTAX(_CLOSE_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* block = Get_Block(token);

    return CTOR_OPER(result_token, condition, block);
}

TreeNode_t* Get_If_oper(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_IF_);
    Token_str* result_token = *token;
    NEXT_TOKEN;

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* condition = Get_Equation(token);

    CHECK_SYNTAX(_CLOSE_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* block = Get_Block(token);

    return CTOR_OPER(result_token, condition, block);
}

TreeNode_t* Get_Assignment(Token_str** token) {
    assert( token);
    assert(*token);

    TreeNode_t* left_hand_side = Get_Variable(token);

    CHECK_SYNTAX(_ASSIGNMENT_);
    Token_str* result_token = *token;
    NEXT_TOKEN;

    TreeNode_t* right_hand_side = Get_Equation(token);

    return CTOR_OPER(result_token, left_hand_side, right_hand_side);
}

TreeNode_t* Get_Equation(Token_str** token) {
    assert( token);
    assert(*token);   

    TreeNode_t* right_son = NULL;
    TreeNode_t* result    = Get_Term(token);

    while( TKN_CODE(*token) == _MATH_ADD_ || TKN_CODE(*token) == _MATH_SUB_ ) 
    {
        Token_str* oper_token = *token;
        NEXT_TOKEN;

        TreeNode_t* right_son = Get_Term(token);

        result = CTOR_OPER(oper_token, result, right_son);          
    }

    return result;
}

TreeNode_t* Get_Term(Token_str** token) {
    assert( token);
    assert(*token);   

    TreeNode_t* result = Get_Power(token);

    while( TKN_CODE(*token) == _MATH_MUL_ || TKN_CODE(*token) == _MATH_DIV_ ) 
    {
        Token_str* oper_token = *token;
        NEXT_TOKEN;

        TreeNode_t* right_son = Get_Power(token);

        result = CTOR_OPER(oper_token, result, right_son);          
    }

    return result;
}

TreeNode_t* Get_Power(Token_str** token) {
    assert( token);
    assert(*token);  

    TreeNode_t* result = Get_Primary(token);

    if ( TKN_CODE(*token) == _MATH_POW_ ) 
    {
        Token_str* oper_token = *token;
        NEXT_TOKEN;

        TreeNode_t* right_son = Get_Primary(token);

        result = CTOR_OPER(oper_token, result, right_son);          
    }

    return result;
}

TreeNode_t* Get_Primary(Token_str** token) {
    assert( token);
    assert(*token);  

    if ( TKN_CODE(*token) == _OPEN_BRACK_ ) 
    {
        NEXT_TOKEN;

        TreeNode_t* result = Get_Equation(token);

        CHECK_SYNTAX(_CLOSE_BRACK_);
        NEXT_TOKEN;

        return result;
    }

    if ( _MATH_UNARY_START_ <= TKN_CODE(*token) && TKN_CODE(*token) <= _MATH_UNARY_END_ )
        return Get_Unary(token);

    if ( isdigit(TKN_NAME(*token)[0]) )
        return Get_Number(token);

    return Get_Variable(token);
}

TreeNode_t* Get_Unary(Token_str** token) {
    assert( token);
    assert(*token);

    Token_str* oper_token = *token;
    NEXT_TOKEN;

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* result = CTOR_OPER(oper_token, Get_Equation(token), NULL);

    CHECK_SYNTAX(_CLOSE_BRACK_);
    NEXT_TOKEN;

    return result;
}

TreeNode_t* Get_Variable(Token_str** token) {
    assert( token);
    assert(*token);

    if ( TKN_CODE(*token) != _UNDEF_TOKEN_) {
        fprintf(stderr, "%s in %s:%d: token almost determined with code %d\n", __func__, __FILE__, __LINE__, TKN_CODE(*token));
        return NULL;
    }

    TKN_CODE(*token) = _VARIABLE_;
    
    TreeNode_t* result = CTOR_VAR(*token);

    NEXT_TOKEN;

    return result;
}

TreeNode_t* Get_Number(Token_str** token) {
    assert( token);
    assert(*token);

    if ( TKN_CODE(*token) != _UNDEF_TOKEN_) {
        fprintf(stderr, "%s in %s:%d: token almost determined with code %d\n", __func__, __FILE__, __LINE__, TKN_CODE(*token));
        return NULL;
    }

    TKN_CODE(*token) = _NUMBER_;
    TKN_VAL(*token)  = strtod( TKN_NAME(*token), NULL);

    TreeNode_t* result = CTOR_NUM(*token);

    NEXT_TOKEN;

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

#undef CTOR_VAR
#undef CTOR_NUM
#undef CTOR_OPER

#undef NEXT_TOKEN
#undef CHECK_SYNTAX