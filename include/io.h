#ifndef IO_H
#define IO_H

FILE* openContentFile(const char* filesPath);

char* readContentFile(FILE* fptr, size_t *bufferSize);

#endif