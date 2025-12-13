#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <settings.h>

static const size_t QR_BYTE_CAPACITY[4][41] = {
    /* L */
    {
        0,
        17,32,53,78,106,134,154,192,230,271,
        321,367,425,458,520,586,644,718,792,858,
        929,1003,1091,1171,1273,1367,1465,1528,1628,1732,
        1840,1952,2068,2188,2303,2431,2563,2699,2809,2953
    },

    /* M */
    {
        0,
        14,26,42,62,84,106,122,152,180,213,
        251,287,331,362,412,450,504,560,624,666,
        711,779,857,911,997,1059,1125,1190,1264,1370,
        1452,1538,1628,1722,1809,1911,1989,2099,2213,2331
    },

    /* Q */
    {
        0,
        11,20,32,46,60,74,86,108,130,151,
        177,203,241,258,292,330,365,415,453,499,
        535,593,647,673,721,795,843,894,953,1012,
        1091,1171,1231,1286,1354,1426,1502,1582,1666,1754
    },

    /* H */
    {
        0,
        7,14,24,34,44,58,64,84,98,119,
        137,155,177,194,220,250,280,310,338,382,
        403,439,461,511,535,593,625,658,698,742,
        790,842,898,958,983,1051,1093,1139,1219,1273
    }
};

typedef struct {
    char** matrix;
    size_t length;
    size_t level;
} Buffer;

Buffer* initBuffer(Settings* stg);

void orientation(Buffer* buffer);

void seperators(Buffer* buffer);

void timing(Buffer* buffer);

void placeOnePixel(Buffer* buffer);

void alignmentPattern(Buffer* buffer);

void encodingMode(Buffer* buffer);

void length(Buffer* buffer, Settings* stg);

void formatInformation(Buffer* buffer, Settings* stg);

void placeData(Buffer* buffer, Settings* stg);

#endif