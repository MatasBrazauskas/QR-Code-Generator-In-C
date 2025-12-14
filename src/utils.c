#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"
#include "settings.h"
#include "errors.h"

static const size_t formatInfoFirst[32] = {
    0x5412, 0x5125, 0x5E7C, 0x5B4B,
    0x45F9, 0x40CE, 0x4F97, 0x4AA0,
    0x77C4, 0x72F3, 0x7DAA, 0x789D,
    0x662F, 0x6318, 0x6C41, 0x6976,
    0x1689, 0x13BE, 0x1CE7, 0x19D0,
    0x0762, 0x0255, 0x0D0C, 0x083B,
    0x355F, 0x3068, 0x3F31, 0x3A06,
    0x24B4, 0x2183, 0x2EDA, 0x2BED
};

static char* formatingInfoSecond[] = {
    "000111110010010100",
    "001000010110111100",
    "001001101010011001",
    "001010010011010011",
    "001011101111110110",
    "001100011101100010",
    "001101100001000111",
    "001110011000001101",
    "001111100100101000",
    "010000101101111000",
    "010001010001011101",
    "010010101000010111",
    "010011010100110010",
    "010100100110100110",
    "010101011010000011",
    "010110100011001001",
    "010111011111101100",
    "011000111011000100",
    "011001000111100001",
    "011010111110101011",
    "011011000010001110",
    "011100110000011010",
    "011101001100111111",
    "011110110101110101",
    "011111001001010000",
    "100000100111010101",
    "100001011011110000",
    "100010100010111010",
    "100011011110011111",
    "100100101100001011",
    "100101010000101110",
    "100110101001100100",
    "100111010101000001",
    "101000110001101001"
};

bool firstMask(int i, int j) {return (i + j) % 2 == 0;}
bool secondMask(int i, int j) {(void)j; return i % 2 == 0;}
bool thirdMask(int i, int j) {(void)i; return j % 3 == 0;}
bool fourthMask(int i, int j) {return (i + j) % 3 == 0;}
bool fifthMask(int i, int j) {return ((i / 2) + (j / 3)) % 2 == 0;}
bool sixthMask(int i, int j) {return (i * j) % 2 + (i * j) % 3 == 0;}
bool seventhMask(int i, int j) {return ((i * j) % 2 + (i * j) % 3) % 2 == 0;}
bool eightMask(int i, int j) {return ((i + j) % 2 + (i * j) % 3) % 2 == 0;} 

typedef bool (*func_type)(int, int);
static func_type functions[] = {
    firstMask,
    &secondMask,
    &thirdMask,
    &fourthMask,
    &sixthMask,
    &seventhMask,
    &eightMask
};

static size_t level(Settings* stg){
    size_t index = 0;

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

    static char pattern[] = "1111111100000110111011011101101110110000011111111";
    static const size_t patternLen = 7;

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
    static const char codes[] = "1111110001101011000111111";

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

void encodingMode(Buffer* buffer, Settings* stg) {
    func_type maskFunction = functions[stg->maskPattern];
    size_t maxLength = buffer->length;

    buffer->matrix[maxLength - 1][maxLength - 1] = 0;
    buffer->matrix[maxLength - 1][maxLength - 2] = 1;
    buffer->matrix[maxLength - 2][maxLength - 1] = 0;
    buffer->matrix[maxLength - 2][maxLength - 2] = 0;

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            if(maskFunction(maxLength - 1 - i, maxLength - 1 - j) == true){
                printf("Hello naxui\n");
                char temp = buffer->matrix[maxLength - 1 - i][maxLength - 1 - j];

                if(temp == 0) buffer->matrix[maxLength - 1 - i][maxLength - 1 - j] = 1;
                else if (temp == 1) buffer->matrix[maxLength - 1 - i][maxLength - 1 - j] = 0;
            }

            buffer->matrix[maxLength - 1 - i][maxLength - 1 - j] += 10;
        }
    }
}

void length(Buffer* buffer, Settings* stg) {
    func_type maskFunction = functions[stg->maskPattern];
    size_t lengthBitCount = 8;

    if(buffer->level >= 10) {
        lengthBitCount = 16;
    }

    size_t maxLength = buffer->length;
    uint16_t lengthInBits = (uint16_t)stg->contentSize;

    for (size_t i = 0; i < lengthBitCount; i++){
        size_t row = maxLength - 2 - (size_t)(lengthBitCount / 2) + (i / 2);
        size_t col = maxLength - 2 + (i % 2);

        bool flag = ((lengthInBits >> i) & 1);
        if(maskFunction(row, col)){
            flag = !flag;
        }

        buffer->matrix[row][col] = flag + 10;
    }
}

void formatInformation(Buffer* buffer, Settings* stg) {
    size_t errorLevel = 0;

    switch(stg->errorCorrectionLevel) {
        case EC_LOW: errorLevel = 1; break;
        case EC_MEDIUM: errorLevel = 0; break;
        case EC_QUATILE: errorLevel = 3; break;
        case EC_HIGH: errorLevel = 2; break;
    }

    size_t formatPattern = formatInfoFirst[stg->maskPattern + 8 * errorLevel];
    int num_bits = sizeof(uint16_t) * 8;
    char *string = malloc(num_bits);

    for (int i = num_bits - 2; i >= 0; i--) {
        string[i] = (formatPattern & 1) + '0';
        formatPattern >>= 1;
    }
    string[num_bits - 1] = '\0';

    size_t maxLength = buffer->length;

    for(size_t i = 0; i < 8; i++) {
        buffer->matrix[8][maxLength - 1 - i] = string[14 - i] - '0' + 12;
    }

    for(size_t i = 0; i < 7; i++) {
        buffer->matrix[maxLength - 1 - i][8] = string[i] - '0' + 12;
    }

    for(size_t i = 0; i < 6; i++){
        buffer->matrix[i][8] = string[14 - i] - '0' + 12;
        buffer->matrix[8][i] = string[i] - '0' + 12;
    }

    buffer->matrix[7][8] = string[8] - '0' + 12;
    buffer->matrix[8][8] = string[7] - '0' + 12;
    buffer->matrix[8][7] = string[6] - '0' + 12;

    if(buffer->level >= 7){
        char* info = formatingInfoSecond[buffer->level - 7];
        size_t maxLength = buffer->length;

        for(size_t i = 0; i < strlen(info); i++) {
            buffer->matrix[maxLength - 9 - (i % 3)][5 - (i / 3)] = info[i] - '0' + 12;
            buffer->matrix[(i / 3)][maxLength - 11 + (i % 3)] = info[18 - 1 - i] - '0' + 12;
        }
    } 

    free(string);
}

void placeData(Buffer* buffer, Settings* stg)
{
    func_type maskFunction = functions[stg->maskPattern];

    int size = buffer->length;
    int x = size - 1;

    size_t charIndex = 0;
    int bitPlacement = 7;

    char mask = 1 << (sizeof(char) * 8 - 1);
    char input_char = stg->content[0];

    for (int i = 0; i < (int)sizeof(char) * 8; i++) {
        if (input_char & mask) {
            printf("1");
        } else {
            printf("0");
        }
        
        mask >>= 1;
    }

    printf("\n");

    while (x > 0 && charIndex < stg->contentSize) {

        if (x == 6) x--;  // skip timing column

        // UP
        for (int y = size - 1; y >= 0; y--) {
            for (int dx = 0; dx < 2; dx++) {

                if (buffer->matrix[y][x - dx] == 0) {
                    bool bit = (stg->content[charIndex] >> bitPlacement) & 1;

                    if (maskFunction(y, x-dx)) {
                        bit ^= 1;
                    }

                    buffer->matrix[y][x - dx] = bit + 14;

                    if (--bitPlacement < 0) {
                        bitPlacement = 7;
                        charIndex++;
                        if (charIndex >= stg->contentSize) return;
                    }
                }
            }
        }

        x -= 2;
        if (x == 6) x--;

        // DOWN
        for (int y = 0; y < size; y++) {
            for (int dx = 0; dx < 2; dx++) {

                if (buffer->matrix[y][x - dx] == 0) {
                    bool bit = (stg->content[charIndex] >> bitPlacement) & 1;

                    if (maskFunction(y, x - dx)) {
                        bit ^= 1;
                    }

                    buffer->matrix[y][x - dx] = bit + 14;

                    if (--bitPlacement < 0) {
                        bitPlacement = 7;
                        charIndex++;
                        if (charIndex >= stg->contentSize) return;
                    }
                }
            }
        }

        x -= 2;
    }
}
