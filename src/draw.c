
#include "draw.h"
#include "errors.h"

static size_t positionPixelCount(){
    return 7 * 7 * 3;
}

static size_t seperatorPixelCount(){
    return (8 + 7) * 3 ;
}

static size_t timingPatternCount(size_t level){
    return 0;
}

static size_t aligmentPatternCount(){
    return 0;
}

static size_t modePixelCount(){
    return 4;
}

static size_t lengthPixelCount(size_t contentSize){
    if(contentSize > 1<<8){
        return 16;
    }
    return 8;
}

static size_t matrixDimentions(Settings* stg){

    size_t size = 0;
    size_t level = 0;

    size += positionPixelCount();
    size += seperatorPixelCount();
    size += timingPatternCount(level);

    return size + 1;
}

Buffer* initBuffer(Settings* stg){
    size_t size = matrixDimentions(stg);

    Buffer* buffer = malloc(sizeof(Buffer));
    
    if(buffer == NULL){
        ExitWithError("Can't allocate memery");
    }

    buffer->sizeLength = size;
    buffer->matrix = malloc(size * sizeof(char*));

    if(buffer->matrix == NULL){
        ExitWithError("Cant allocate");
    }

    for(size_t i = 0; i < size; i++){
        buffer->matrix[i] = malloc(size * sizeof(char));

        if(buffer->matrix[i] == NULL){
            ExitWithError("Buffer cant allocate");
        }
    }

    return buffer;
}

void PositionOrientationFinders() {

}