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
    printf("%s %zu\n", MASK_FLAG, stg->maskPattern);

    Buffer* buffer = initBuffer(stg);

    orientation(buffer);
    seperators(buffer);
    timing(buffer);
    placeOnePixel(buffer);
    alignmentPattern(buffer);
    encodingMode(buffer, stg);
    length(buffer, stg);
    formatInformation(buffer, stg);

    placeData(buffer, stg);

    createImage(buffer, stg);

    for(int i = 0; i < (int)buffer->length; i++){
        for(int j = 0; j < (int)buffer->length; j++){
            printf("%d ", buffer->matrix[i][j]);
        }
        printf("\n");
    }

    printf("Version %zu\n", buffer->level);
    printf("Buffer length %zu\n", buffer->length);
}