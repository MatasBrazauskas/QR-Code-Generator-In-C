#ifndef SETTINGS_H
#define SETTINGS_H

#define SIZE_FLAG "-s"
#define ERR_LEVEL_FLAG "-e"
#define CONTENT_FILE_FLAG "-f"
#define CONTENT_TEXT_FLAG "-t"
#define ENCODING_MODE_FLAG "-m"

typedef enum {
    EC_LOW = 'l',
    EC_MEDIUM = 'm',
    EC_QUATILE = 'q',
    EC_HIGH = 'h',
    EC_AUTOMATIC = 'a'
} ErrorCorrection;

typedef enum {
    W_SMALL = 's',
    W_MEDIUM = 'm',
    W_LARGE = 'l'
} WindowSize;

typedef enum {
    M_NUM = 'n',
    M_ALPHA = 'a',
    M_BYTE = 'b'
} EncodingMode;

typedef struct {
    WindowSize winSize;
    ErrorCorrection errLevel;
    EncodingMode ecMode;
    char* content;
    size_t contentSize;
} Settings;

Settings* initSettings();

FILE* openContentFile(char* filesPath);

char* readContentFile(FILE* fptr, size_t *bufferSize);

Settings* getSettings(int argc, char** argv);


#endif
