#ifndef ERRORS_H
#define ERRORS_H

#define OUT_OF_BOUNDS "Can't accsses argument. Out of bounds"
#define NOT_ENOUGH_ARGUMENTS "Not enough arguments!"
#define ARGUMENT_TOO_SHORT "Argument is too short"
#define UNKNOWN_TYPE "Arguments type is unknown"

void ExitWithError(char* mess);

void ArgvBoundCheck(size_t index, int argc);

#endif