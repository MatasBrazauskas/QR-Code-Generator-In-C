#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "settings.h"
#include "errors.h"
#include "io.h"

static Settings* initSettings(){
    Settings* stg = malloc(sizeof(Settings));

    if(stg == NULL){
        ExitWithError("Can't  allocate settings");
    }

    stg->windowSize =W_MEDIUM;
    stg->errorCorrectionLevel = EC_AUTOMATIC;
    stg->encodingMode = M_BYTE;
    stg->colorMode = WHITE_BLACK;
    stg->content = NULL;
    stg->contentSize = 0;

    return stg;
}

Settings* getSettings(int argc, char** argv) {
    Settings* stg = initSettings();

    for(size_t i = 1; i < (size_t)argc; ++i)
    {
        if(strcmp(argv[i], SIZE_FLAG) == 0){
            ArgvBoundCheck(i, argc);

            char windowSize = tolower(argv[i + 1][0]);
            
            switch(windowSize) {
                case W_SMALL: stg->windowSize = W_SMALL; break;
                case W_MEDIUM: stg->windowSize = W_MEDIUM; break;
                case W_LARGE: stg->windowSize = W_LARGE; break;
                default:
                    ExitWithError(UNKNOWN_TYPE);
            }
            i++;
        }
        else if(strcmp(argv[i], CONTENT_FILE_FLAG) == 0){
            ArgvBoundCheck(i, argc);

            const char* path = argv[i + 1];

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
                case M_NUM: stg->encodingMode = M_NUM; break;
                case M_ALPHA: stg->encodingMode = M_ALPHA; break;
                case M_BYTE: stg->encodingMode = M_BYTE; break;
                default:
                    ExitWithError(UNKNOWN_TYPE);
            }

            i++;
        }
        else if(strcmp(argv[i], ERR_LEVEL_FLAG) == 0){
            ArgvBoundCheck(i, argc);

            if(strlen(argv[i + 1]) != 1){
                ExitWithError(ARGUMENT_TOO_SHORT);
            }

            char errorCorrection = tolower(argv[i + 1][0]);

            switch(errorCorrection){
                case EC_LOW: stg->errorCorrectionLevel = EC_LOW; break;
                case EC_MEDIUM: stg->errorCorrectionLevel = EC_MEDIUM; break;
                case EC_QUATILE: stg->errorCorrectionLevel = EC_QUATILE; break;
                case EC_HIGH: stg->errorCorrectionLevel = EC_HIGH; break;
                default:
                    stg->errorCorrectionLevel = EC_AUTOMATIC;
            }

            i++;
        }
        else if(strcmp(argv[i], COLOR_MODE_FLAG) == 0){
            stg->colorMode = COLOR;
        }
        else {
            ExitWithError(UNKNOWN_TYPE);
        }

    }

    return stg;
}
