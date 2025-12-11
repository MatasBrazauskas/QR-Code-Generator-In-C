#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "settings.h"
#include "errors.h"

static int level(Settings* stg){
    size_t index;

    switch(stg->errorCorrectionLevel) {
        case EC_LOW: index = 0; break;
        case EC_MEDIUM: index = 1; break;
        case EC_QUATILE: index = 2; break;
        case EC_HIGH: index = 3; break;
        default:
            ExitWithError("Wrong Error correction level");
    }

    for(size_t i = 1; i < 40; i++){
        if(QR_BYTE_CAPACITY[index][i] >= stg->contentSize){
            return i;
        }
    }

    return -1;
}

Buffer* initBuffer(Settings* stg) {
    Buffer* buffer = malloc(sizeof(Buffer));

    int lvl = level(stg);

    if(lvl == -1){
        ExitWithError("Can't get the level");
    }

    buffer->level= (size_t)lvl;
    buffer->length = 21 + 4 * (lvl - 1);
    buffer->matrix = malloc(sizeof(char*) * buffer->length);

    for(size_t i = 0; i < buffer->length; i++){
        buffer->matrix[i] = malloc(sizeof(char) * buffer->length);
    }

    return buffer;
}

void orientation(Buffer* buffer) {

    const char pattern[] = "1111111100000110111011011101101110110000011111111";
    const size_t patternLen = 7;

    for(size_t i = 0; i < patternLen; i++){
        for(size_t j = 0; j < patternLen; j++) {
            buffer->matrix[0 + i][0 + j] = (pattern[i * patternLen + j] * 1 -'0')+ 2;
            buffer->matrix[buffer->length - patternLen + i][0 + j] = (pattern[i * patternLen + j] * 1 - '0') + 2;
            buffer->matrix[0 + i][buffer->length - patternLen + j] = (pattern[i * patternLen + j] * 1 - '0') + 2;
        }
    }
}

void seperators(Buffer* buffer)
{
    for(int i = 0; i < 8; i++)
    {

        buffer->matrix[i][7] = 4;
        buffer->matrix[i][buffer->length - 8] = 4;
        buffer->matrix[i + buffer->length - 8][7] = 4;
    }

    for(int i = 0; i < 7; i++)
    {
        buffer->matrix[7][i] = 4;
        buffer->matrix[7][buffer->length - 7 + i] = 4;
        buffer->matrix[buffer->length - 8][i] = 4;
    }
}

void timing(Buffer* buffer)
{
    for(size_t i = 8; i < buffer->length - 8; i++)
    {
        buffer->matrix[i][6] = (i % 2 == 0) ? 7 : 6;
        buffer->matrix[6][i] = buffer->matrix[i][6];
    }
}

void placeOnePixel(Buffer* buffer)
{
    buffer->matrix[buffer->length - 8][8] = 1;
}