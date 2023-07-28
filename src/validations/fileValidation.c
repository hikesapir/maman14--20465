#include <string.h>
#include <stdio.h>
#include "fileValidation.h"
#include "../logger/logger.h"
#include "../utils/utils.h"

FILE *validateFile(char *fileName)
{
    FILE *source_file;
    char fullFileName[PATH_MAX];

    strcpy(fullFileName, fileName); /* Copy fileName into fullFileName */
    strcat(fullFileName, ".as");    /* Add a .as to fullFileName */

    logInfo("oppening %s", fullFileName);
    source_file = fopen(fullFileName, "r"); /* read only */

    /* test for file not existing. */
    if (source_file == NULL)
        logError("Could not open file %s", fullFileName);
    else
        logInfo("File opened successfully!");

    return source_file;
}