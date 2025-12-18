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

    if ( TKN_CODE(*token) == _ELSE_)
        return Get_Else_oper(token);

    if ( TKN_CODE(*token) == _WHILE_)
        return Get_While_oper(token);

    if ( TKN_CODE(*token) == _RETURN_)
        return Get_Return_oper(token);
    
    if ( TKN_CODE(*token) == _PRINT_)
        return Get_Print_oper(token);
    
    if ( TKN_CODE(*token) == _VAR_INIT_)
        return Get_Var_Init(token);

    if ( TKN_CODE(*token) == _FUNC_INIT_)
        return Get_Func_Init(token);

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

TreeNode_t* Get_Func_Init(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_FUNC_INIT_);
    NEXT_TOKEN;

    TreeNode_t* result      = Get_Identifier(token);
    TKN_CODE(result->token) = _FUNC_INIT_;

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* args = Get_Variable(token);

    while( TKN_CODE(*token) == _COMMA_ ) 
    {
        Token_str* oper_token = *token;
        NEXT_TOKEN;

        TreeNode_t* right_son = Get_Variable(token);

        args = CTOR_OPER(oper_token, args, right_son);          
    }

    CHECK_SYNTAX(_CLOSE_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* body = Get_Block(token);

    LEFT(result)  = args;
    RIGHT(result) = body;

    return result;
}

TreeNode_t* Get_Var_Init(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_VAR_INIT_);
    NEXT_TOKEN;

    TreeNode_t* result      = Get_Variable(token);
    TKN_CODE(result->token) = _VAR_INIT_;

    if ( TKN_CODE(*token) == _ASSIGNMENT_ )
    {
        Token_str* result_token = *token;
        NEXT_TOKEN;   

        TreeNode_t* right_hand_side = Get_Logical(token);

        result = CTOR_OPER(result_token, result, right_hand_side);
    }

    return result;
}

TreeNode_t* Get_Return_oper(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_RETURN_);
    Token_str* result_token = *token;
    NEXT_TOKEN;    

    TreeNode_t* value = Get_Expression(token);

    return CTOR_OPER(result_token, value, NULL);   
}

TreeNode_t* Get_Print_oper(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_PRINT_);
    Token_str* result_token = *token;
    NEXT_TOKEN;    

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* value = Get_Expression(token);

    CHECK_SYNTAX(_CLOSE_BRACK_);
    NEXT_TOKEN;

    return CTOR_OPER(result_token, value, NULL);   
}

TreeNode_t* Get_While_oper(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_WHILE_);
    Token_str* result_token = *token;
    NEXT_TOKEN;

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* condition = Get_Logical(token);

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

    TreeNode_t* condition = Get_Logical(token);

    CHECK_SYNTAX(_CLOSE_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* block = Get_Block(token);

    return CTOR_OPER(result_token, condition, block);
}

TreeNode_t* Get_Else_oper(Token_str** token) {
    assert( token);
    assert(*token);

    CHECK_SYNTAX(_ELSE_);
    Token_str* result_token = *token;
    NEXT_TOKEN;

    TreeNode_t* block = Get_Block(token);

    return CTOR_OPER(result_token, block, NULL);
}

TreeNode_t* Get_Assignment(Token_str** token) {
    assert( token);
    assert(*token);

    TreeNode_t* left_hand_side = Get_Identifier(token);

    CHECK_SYNTAX(_ASSIGNMENT_);
    Token_str* result_token = *token;
    NEXT_TOKEN;

    TreeNode_t* right_hand_side = Get_Logical(token);

    return CTOR_OPER(result_token, left_hand_side, right_hand_side);
}

TreeNode_t* Get_Logical(Token_str** token) {
    assert( token);
    assert(*token);   

    TreeNode_t* right_son = NULL;
    TreeNode_t* result    = Get_Expression(token);

    while( TKN_CODE(*token) == _LOG_EQUAL_ || TKN_CODE(*token) == _LOG_MORE_ || TKN_CODE(*token) == _LOG_LESS_ ) 
    {
        Token_str* oper_token = *token;
        NEXT_TOKEN;

        TreeNode_t* right_son = Get_Expression(token);

        result = CTOR_OPER(oper_token, result, right_son);          
    }

    return result;
}

TreeNode_t* Get_Expression(Token_str** token) {
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

        TreeNode_t* result = Get_Expression(token);

        CHECK_SYNTAX(_CLOSE_BRACK_);
        NEXT_TOKEN;

        return result;
    }

    if ( _MATH_UNARY_START_ <= TKN_CODE(*token) && TKN_CODE(*token) <= _MATH_UNARY_END_ )
        return Get_Unary(token);

    if ( isdigit(TKN_NAME(*token)[0]) )
        return Get_Number(token);

    if ( (*token + 1)->code == _OPEN_BRACK_ )
        return Get_Function(token);
        
    return Get_Identifier(token);
}

TreeNode_t* Get_Unary(Token_str** token) {
    assert( token);
    assert(*token);

    Token_str* oper_token = *token;
    NEXT_TOKEN;

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* result = CTOR_OPER(oper_token, Get_Expression(token), NULL);

    CHECK_SYNTAX(_CLOSE_BRACK_);
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

TreeNode_t* Get_Variable(Token_str** token) {
    assert( token);
    assert(*token);

    TreeNode_t* result      = Get_Identifier(token);
    TKN_CODE(result->token) =  _VARIABLE_;
    
    return result;
}

TreeNode_t* Get_Function(Token_str** token) {
    assert( token);
    assert(*token);

    TreeNode_t* result      = Get_Identifier(token);
    TKN_CODE(result->token) =  _FUNCTION_;

    CHECK_SYNTAX(_OPEN_BRACK_);
    NEXT_TOKEN;

    TreeNode_t* args = Get_Expression(token);

    while( TKN_CODE(*token) == _COMMA_ ) 
    {
        Token_str* oper_token = *token;
        NEXT_TOKEN;

        TreeNode_t* right_son = Get_Expression(token);

        args = CTOR_OPER(oper_token, args, right_son);          
    }

    CHECK_SYNTAX(_CLOSE_BRACK_);
    NEXT_TOKEN;

    LEFT(result)  = args;
    RIGHT(result) = NULL;

    return result;
}

TreeNode_t* Get_Identifier(Token_str** token) {
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

//---------------------------------------------------------------------------------------------------------------------

#undef CTOR_VAR
#undef CTOR_NUM
#undef CTOR_OPER

#undef NEXT_TOKEN
#undef CHECK_SYNTAX