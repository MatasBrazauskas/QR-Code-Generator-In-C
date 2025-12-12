#ifndef SETTINGS_H
#define SETTINGS_H

#define SIZE_FLAG "--size"
#define ERR_LEVEL_FLAG "--error"
#define CONTENT_FILE_FLAG "--file"
#define CONTENT_TEXT_FLAG "--text"
#define COLOR_MODE_FLAG "--color"

typedef enum {
    EC_LOW = 'l',
    EC_MEDIUM = 'm',
    EC_QUATILE = 'q',
    EC_HIGH = 'h',
} ErrorCorrection;

typedef enum {
    WHITE_BLACK,
    COLOR
} ColorMode;

typedef struct {
    size_t windowSize;
    ErrorCorrection errorCorrectionLevel;
    ColorMode colorMode;
    char* content;
    size_t contentSize;
} Settings;

Settings* getSettings(int argc, char** argv);

#endif
