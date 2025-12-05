#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "lexical.h"

FILE* Logfile = fopen("Logfile.htm", "w");

void Logfile_close(void) {
    fclose(Logfile);
}

int main() {
    assert(Logfile);

    Test_Grammar("disk_input.txt");

    atexit(Logfile_close);
    return 0;
}