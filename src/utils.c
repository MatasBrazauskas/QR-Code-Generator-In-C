#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"
#include "settings.h"
#include "errors.h"

static size_t level(Settings* stg){
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

    if(lvl == -1) ExitWithError("Can't get the level");

    buffer->level= lvl;
    buffer->length = 21 + 4 * (lvl - 1);
    buffer->matrix = malloc(sizeof(char*) * buffer->length);

    for(size_t i = 0; i < buffer->length; i++){
        buffer->matrix[i] = malloc(sizeof(char) * buffer->length);

        for(size_t j = 0; j < buffer->length; j++){
            buffer->matrix[i][j] = 0;
        }
    }

    return buffer;
}

void orientation(Buffer* buffer) {

    const char pattern[] = "1111111100000110111011011101101110110000011111111";
    const size_t patternLen = 7;

    for(size_t i = 0; i < patternLen; i++){
        for(size_t j = 0; j < patternLen; j++) {
            buffer->matrix[0 + i][0 + j] = (pattern[i * patternLen + j] * 1 -'0') + 2;
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

void alignmentPattern(Buffer* buffer)
{
    if(buffer->level == 1) return;

    int count = (int)floor(buffer->level / 7.0f) + 2;
    int step = (int)floor((buffer->length - 13) / (count - 1));

    int coordinates[count];
    static char codes[] = "1111110001101011000111111";

    for(int i = 0; i < count; i++)
    {
        coordinates[i] = 6 + step * i;
    }

    for(int i = 0; i < count; i++)
    {
        for(int j = 0; j < count; j++)
        {
            int x = coordinates[i];
            int y = coordinates[j];
            size_t val = buffer->matrix[x][y];
            if(val == 0 || val == 7) {
                for(size_t k = 0; k < 25; k++){
                    buffer->matrix[x - 2 + (k / 5)][y - 2 + (k % 5)] = (codes[k] * 1) + 8 - '0';
                }
            }
        }
    }
}

void encodingMode(Buffer* buffer) {
    size_t maxLength = buffer->length;
    buffer->matrix[maxLength - 1][maxLength - 1] = 10;
    buffer->matrix[maxLength - 1][maxLength - 2] = 11;
    buffer->matrix[maxLength - 2][maxLength - 1] = 10;
    buffer->matrix[maxLength - 2][maxLength - 2] = 10;
}
