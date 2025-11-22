#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define SIZE_FLAG "-s"
#define ERR_LEVEL_FLAG "-e"
#define CONTENT_FILE_FLAG "-f"
#define CONTENT_TEXT_FLAG "-t"
#define ENCODING_MODE_FLAG "-m"

#define OUT_OF_BOUNDS "Can't accsses argument. Out of bounds"
#define NOT_ENOUGH_ARGUMENTS "Not enough arguments!"
#define ARGUMENT_TOO_SHORT "Argument is too short"
#define UNKNOWN_TYPE "Arguments type is unknown"

#define MAX_SIZE 1024

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

Settings* initSettings(){
    Settings* stg = malloc(sizeof(Settings));

    stg->winSize =W_MEDIUM;
    stg->errLevel = EC_AUTOMATIC;
    stg->ecMode = M_BYTE;
    stg->content = "";
    stg->contentSize = 0;
}

void ExitWithError(char* mess) {
    fprintf(stderr, "%s\n", mess);
    exit(1);
}

void ArgvBoundCheck(size_t index, int argc){
    if(index + 1 >= argc){
        ExitWithError(OUT_OF_BOUNDS);
    }
}

FILE* openContentFile(char* filesPath){
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

    char* buffer = malloc(size + 1);
    if(buffer == NULL){
        ExitWithError("Can't allocate buffer");
    }

    size_t read = fread(buffer, 1, size, fptr);
    buffer[read] = '\0';

    *bufferSize = read;
    return buffer;
}

Settings* getSettings(int argc, char** argv) {
    Settings* stg = initSettings();

    for(size_t i = 1; i < argc; ++i)
    {
        if(strcmp(argv[i], SIZE_FLAG) == 0){
            ArgvBoundCheck(i, argc);

            char windowSize = tolower(argv[i + 1][0]);
            
            switch(windowSize) {
                case W_SMALL: stg->winSize = W_SMALL; break;
                case W_MEDIUM: stg->winSize = W_MEDIUM; break;
                case W_LARGE: stg->winSize = W_LARGE; break;
                default:
                    ExitWithError(UNKNOWN_TYPE);
            }
            i++;
        }
        else if(strcmp(argv[i], CONTENT_FILE_FLAG) == 0){
            ArgvBoundCheck(i, argc);

            char* path = argv[i + 1];

            if (strlen(argv[i + 1]) == 0) {
                 ExitWithError(ARGUMENT_TOO_SHORT);
            }

            FILE* fptr = openContentFile(path);

            size_t bufferSize;
            char* content = readContentFile(fptr, &bufferSize);

            stg->content = content;
            stg->contentSize = bufferSize;

            fclose(fptr);
            i++;

        }
        else if(strcmp(argv[i], CONTENT_TEXT_FLAG) == 0) {
            ArgvBoundCheck(i, argc);

            char* buffer = argv[i + 1];
            size_t bufferSize = strlen(buffer);

            stg->content = buffer;
            stg->contentSize = bufferSize;
            i++;
        }
        else if (strcmp(argv[i], ENCODING_MODE_FLAG) == 0){
            ArgvBoundCheck(i, argc);

            if(strlen(argv[i+1]) != 1){
                ExitWithError(ARGUMENT_TOO_SHORT);
            }

            char encodingFlag = tolower(argv[i + 1][0]); 

            switch(encodingFlag) {
                case M_NUM: stg->ecMode = M_NUM; break;
                case M_ALPHA: stg->ecMode = M_ALPHA; break;
                case M_BYTE: stg->ecMode = M_BYTE; break;
                default:
                    ExitWithError(UNKNOWN_TYPE);
            }

            i++;
        }
    }

    return stg;
}

int main(int argc, char** argv)
{
    Settings* stg = getSettings(argc, argv);
    printf("%c\n", stg->winSize);
    printf("%s\n", stg->content);
    printf("%c\n", stg->ecMode);

    free(stg);
}
