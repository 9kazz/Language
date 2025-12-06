#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "types.h"
#include "Logfile.h"
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