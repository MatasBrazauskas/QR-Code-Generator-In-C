#include <stdio.h>
#include <stdlib.h>

#include "settings.h"
#include "errors.h"
#include "utils.h"
#include "io.h"

int main(int argc, char** argv)
{
    Settings* stg = getSettings(argc, argv);

    printf("%s %zu\n", SIZE_FLAG, stg->windowSize);
    printf("%s %s\n",CONTENT_TEXT_FLAG, stg->content);
    printf("%s %zu\n", "SIZE OF CONTENT", stg->contentSize);
    printf("%s %c\n", ERR_LEVEL_FLAG, stg->errorCorrectionLevel);

    Buffer* buffer = initBuffer(stg);

    orientation(buffer);
    seperators(buffer);
    timing(buffer);
    placeOnePixel(buffer);
    alignmentPattern(buffer);
    encodingMode(buffer);

    createImage(buffer, stg);

    for(int i = 0; i < (int)buffer->length; i++){
        for(int j = 0; j < (int)buffer->length; j++){
            printf("%d ", buffer->matrix[i][j]);
        }
        printf("\n");
    }

    printf("%zu\n", buffer->level);
    printf("%zu\n", buffer->length);
}