#include <string.h>
#include <stdio.h>
#include "fileValidation.h"

/* check that the file is accessible */
FILE *validateFile(char *fileName)
{
    FILE *out_file;
    char fullFileName[120];

    strcpy(fullFileName, fileName); /* Copy fileName into fullFileName */
    strcat(fullFileName, ".as");    /* Add a .as to fullFileName */

    printf("oppening %s\n", fullFileName);
    out_file = fopen(fullFileName, "r"); /* read only */

    /* test for file not existing. */
    if (out_file == NULL)
    {
        printf("Error! Could not open file\n");
        return NULL;
    }
    else
    {
        printf("File opened successfully!\n");
        return out_file;
    }
}