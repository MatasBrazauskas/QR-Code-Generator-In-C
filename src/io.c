#include <stdlib.h>
#include <stdio.h>

#include "errors.h"
#include "io.h"

FILE* openContentFile(const char* filesPath){
    FILE* fptr = fopen(filesPath, "r");

    if(fptr == NULL){
        ExitWithError("Can't open file");
    }

    return fptr;
}

char* readContentFile(FILE* fptr, size_t *bufferSize){

    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    rewind(fptr);

    if(size < 0) {
        ExitWithError("Can't read file");
    }

    char* buffer = malloc((size_t)size + 1);
    if(buffer == NULL){
        ExitWithError("Can't allocate buffer");
    }

    size_t read = fread(buffer, 1, size, fptr);
    buffer[read] = '\0';

    *bufferSize = read;

    return buffer;
}