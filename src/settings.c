#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "settings.h"
#include "errors.h"

Settings* initSettings(){
    Settings* stg = malloc(sizeof(Settings));

    stg->winSize =W_MEDIUM;
    stg->errLevel = EC_AUTOMATIC;
    stg->ecMode = M_BYTE;
    stg->content = "";
    stg->contentSize = 0;

    return stg;
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

    for(size_t i = 1; i < (size_t)argc; ++i)
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
