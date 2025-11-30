#include <stdio.h>
#include <stdlib.h>

#include "settings.h"
#include "errors.h"
#include "draw.h"

int main(int argc, char** argv)
{
    Settings* stg = getSettings(argc, argv);

    printf("%s %c\n", SIZE_FLAG, stg->windowSize);
    printf("%s %s\n",CONTENT_TEXT_FLAG, stg->content);
    printf("%s %c\n", ENCODING_MODE_FLAG, stg->encodingMode);
    printf("%s %c\n", ERR_LEVEL_FLAG, stg->errorCorrectionLevel);
    printf("%s %d\n", COLOR_MODE_FLAG, stg->colorMode);

    Buffer* buffer = initBuffer(stg);

    printf("%zu\n", buffer->sizeLength);
}
