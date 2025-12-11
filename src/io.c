#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "errors.h"
#include "io.h"

static char* Colors[] = {
    "255 255 255",
    "0 0 0",
    "255 128 128",
    "127 0 0",
    "128 213 128",
    "NULL",
    "127 51 0",
    "255 179 128"
};


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

char* colorCode(size_t index, Settings* stg){
    if(stg->colorMode == WHITE_BLACK){
        return Colors[index % 2];
    }
    return Colors[index];
}

void createImage(Buffer* buffer, Settings* stg) {
    FILE* fptr = fopen("temp.ppm", "w");
    if (!fptr) ExitWithError("Can't create PBM");

    const size_t PADDING = 4;

    size_t qrSize = buffer->length;
    size_t fullModules = qrSize + 2 * PADDING;

    size_t pixelsPerModule = stg->windowSize / fullModules;

    if (pixelsPerModule < 1)
        pixelsPerModule = 1;

    size_t imageSize = fullModules * pixelsPerModule;

    fprintf(fptr, "P3\n");
    fprintf(fptr, "%zu %zu\n", imageSize, imageSize);
    fprintf(fptr, "%d\n", 255);

    for (size_t y = 0; y < imageSize; y++) {
        size_t moduleY = y / pixelsPerModule;

        for (size_t x = 0; x < imageSize; x++) {
            size_t moduleX = x / pixelsPerModule;

            char* val = Colors[0];

            if (moduleX >= PADDING && moduleX < PADDING + qrSize &&
                moduleY >= PADDING && moduleY < PADDING + qrSize) {

                size_t qrX = moduleX - PADDING;
                size_t qrY = moduleY - PADDING;

                val = colorCode(buffer->matrix[qrY][qrX], stg);
            }

            fprintf(fptr, "%s ", val);
            fputc('\n', fptr);
        }

    }

    fclose(fptr);
}