#ifndef IO_H
#define IO_H

#include "utils.h"

FILE* openContentFile(const char* filesPath);

char* readContentFile(FILE* fptr, size_t *bufferSize);

void createImage(Buffer* buffer, Settings* stg);

#endif