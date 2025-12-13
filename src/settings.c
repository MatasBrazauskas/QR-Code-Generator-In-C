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

    stg->windowSize = 600;
    stg->errorCorrectionLevel = EC_HIGH;
    stg->maskPattern = 0;
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
            stg->windowSize = atol(argv[i + 1]);
            i++;
        }
        else if(strcmp(argv[i], MASK_FLAG) == 0){
            ArgvBoundCheck(i, argc);
            int maskPattern = atoi(argv[i + 1]);

            if(maskPattern < 0 || maskPattern > 7){
                ExitWithError("Mask pattern is incorrect");
            }

            stg->maskPattern = (size_t)maskPattern;
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
                    ExitWithError("Unknown error flag");
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
