#include <stdlib.h>
#include <stdio.h>

#include "errors.h"

void ExitWithError(char* mess) {
    fprintf(stderr, "%s\n", mess);
    exit(1);
}

void ArgvBoundCheck(size_t index, int argc){
    if(index + 1 >= (size_t)argc){
        ExitWithError(OUT_OF_BOUNDS);
    }
}
