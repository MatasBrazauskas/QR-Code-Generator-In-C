#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "errors.h"
#include "io.h"

static char* Colors[] = {
    "255 255 255", //white
    "0 0 0", //black
    "255 128 128", // postion L
    "127 0 0", //postion D
    "128 213 128", //seperator
    "NULL", // empty
    "127 51 0", // timing L
    "255 179 128", // timing D
    "255 170 204", // aligment L 
    "127 42 76", // aligment D
    "240 155 255", // enc and length L
    "127 42 76", // enc and length D
    "137 234 223", //formating L
    "178 60 0" // fomrating D
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

    char* buffer = malloc((size_t)size);
    if(buffer == NULL){
        ExitWithError("Can't allocate buffer");
    }

    size_t read = fread(buffer, 1, size, fptr);

    *bufferSize = read;

    return buffer;
}

void createImage(Buffer* buffer, Settings* stg) {
    if(stg->colorMode == WHITE_BLACK) {
        createImageWhiteBlack(buffer, stg);
    }else {
        createImageColorful(buffer, stg);
    }
}

void createImageColorful(Buffer* buffer, Settings* stg) {
    FILE* fptr = fopen("temp.ppm", "w");
    if (!fptr) ExitWithError("Can't create PPM");

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

                size_t index = buffer->matrix[qrY][qrX];
                val = Colors[index];
            }
            fprintf(fptr, "%s\n", val);
        }
    }
    fclose(fptr);
}


void createImageWhiteBlack(Buffer* buffer, Settings* stg) {
    FILE* fptr = fopen("temp.pbm", "w");
    if (!fptr) ExitWithError("Can't create PBM");

    const size_t PADDING = 4;

    size_t qrSize = buffer->length;
    size_t fullModules = qrSize + 2 * PADDING;

    size_t pixelsPerModule = stg->windowSize / fullModules;

    if (pixelsPerModule < 1)
        pixelsPerModule = 1;

    size_t imageSize = fullModules * pixelsPerModule;

    fprintf(fptr, "P1\n");
    fprintf(fptr, "%zu %zu\n", imageSize, imageSize);

    for (size_t y = 0; y < imageSize; y++) {
        size_t moduleY = y / pixelsPerModule;

        for (size_t x = 0; x < imageSize; x++) {
            size_t moduleX = x / pixelsPerModule;
            char val = 0;

            if (moduleX >= PADDING && moduleX < PADDING + qrSize && moduleY >= PADDING && moduleY < PADDING + qrSize) {
                size_t qrX = moduleX - PADDING;
                size_t qrY = moduleY - PADDING;

                val = buffer->matrix[qrY][qrX] % 2;
            }

            fprintf(fptr, "%d ", val);
        }
        fputc('\n', fptr);

    }

    fclose(fptr);
}