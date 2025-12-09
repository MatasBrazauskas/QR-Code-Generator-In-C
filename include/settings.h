#ifndef SETTINGS_H
#define SETTINGS_H

#define SIZE_FLAG "--size"
#define ERR_LEVEL_FLAG "--error"
#define CONTENT_FILE_FLAG "--file"
#define CONTENT_TEXT_FLAG "--text"
//#define ENCODING_MODE_FLAG "--mode"
#define COLOR_MODE_FLAG "--color"

typedef enum {
    EC_LOW = 'l',
    EC_MEDIUM = 'm',
    EC_QUATILE = 'q',
    EC_HIGH = 'h',
    //EC_AUTOMATIC,
} ErrorCorrection;

/*typedef enum {
    W_SMALL = 's',
    W_MEDIUM = 'm',
    W_LARGE = 'l'
} WindowSize;*/

/*typedef enum {
    M_NUM = 'n',
    M_ALPHA = 'a',
    M_BYTE = 'b'
} EncodingMode;*/

typedef enum {
    WHITE_BLACK,
    COLOR
} ColorMode;

typedef struct {
    //WindowSize windowSize;
    size_t windowSize;
    ErrorCorrection errorCorrectionLevel;
    //EncodingMode encodingMode;
    ColorMode colorMode;
    char* content;
    size_t contentSize;
} Settings;

Settings* getSettings(int argc, char** argv);

#endif
