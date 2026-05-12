#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "Logfile.h"
#include "utils.h"
#include "tree.h"
#include "types.h"

int skip_space(char* buffer, size_t size_of_buf, size_t cur_idx) {
    assert(buffer);

    if ( ! buffer ) {
        fprintf(stderr, "skip_space: NULL pointer to Stream file\n");
        return -1;
    }

    if (cur_idx >= size_of_buf) {
        fprintf(stderr, "skip_space: cur_idx out of bufferfile\n");
        return -1;
    }

    size_t idx = cur_idx;
    int count_of_space = 0;

    while( idx < size_of_buf &&
           isspace( buffer[idx]) ) 
    {
        count_of_space++ ;
        idx++ ;
    }

    return count_of_space;
}