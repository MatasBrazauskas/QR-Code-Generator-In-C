#include <stdio.h>
#include <stdlib.h>

#include "settings.h"
#include "errors.h"

int main(int argc, char** argv)
{
    Settings* stg = getSettings(argc, argv);
    printf("%c\n", stg->winSize);
    printf("%s\n", stg->content);
    printf("%c\n", stg->ecMode);

    free(stg);
}
