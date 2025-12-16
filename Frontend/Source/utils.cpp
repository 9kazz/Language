#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "types.h"
#include "Logfiles.h"
#include "utils.h"
#include "verify.h"

size_t skip_space(char** str) {
    assert(str);
    assert(*str);
    
    int count_of_lines = 0;

    while( isspace(**str) ) 
    {
        if (**str == '\n')
            count_of_lines++ ;

        (*str)++ ;
    }

    return count_of_lines;
}

Compare Double_compare(double num_1, double num_2) {

    const double EPSILON = 1e-6;
    double delta = num_1 - num_2;

    if (delta > EPSILON)
        return MORE;

    else if (delta < -EPSILON)
        return LESS;

    return EQUAL;
}

Data_Type Node_Type(const TreeNode_t* node) {
    assert(node);
    
    if ( TKN_CODE(node->token) == -1 )
        return TYPE_VAR;
    
    if ( TKN_CODE(node->token) == -2 ) 
            return TYPE_FUNC;

    if ( TKN_CODE(node->token) == -3 )
        return TYPE_NUM;

    if ( TKN_CODE(node->token) == 1 )
        return TYPE_FUNC_INIT;

    if ( TKN_CODE(node->token) == 2)
        return TYPE_VAR_INIT;

    if ( TKN_CODE(node->token) > 2 )
        return TYPE_OPER;

    return TYPE_UNKNOWN;
}