#include <string.h>
#include <stdio.h>
#include "fileValidation.h"
#include "../logger/logger.h"

FILE *validateFile(char *fileName)
{
    FILE *source_file;
    char fullFileName[120];

    strcpy(fullFileName, fileName); /* Copy fileName into fullFileName */
    strcat(fullFileName, ".as");    /* Add a .as to fullFileName */

    logInfo("oppening %s\n", fullFileName);
    source_file = fopen(fullFileName, "r"); /* read only */

    /* test for file not existing. */
    if (source_file == NULL)
        logError("Error! Could not open file %s\n", fullFileName);
    else
        logInfo("File opened successfully!\n");

    return source_file;
}