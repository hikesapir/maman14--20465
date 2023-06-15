#include <stdio.h>
#include <stdlib.h>
#include "src/validations/fileValidation.h"
#include "src/logger/logger.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *file;

    /* Run the script */
    if (argc == 1)
    {
        logError("No filename entered :(\n");
        exit(-1);
    }

    for (i = 1; i < argc; ++i)
    {
        file = validateFile(argv[i]);
        if (file != NULL)
        {
            /* START OF FILE USAGE */
            /*...*/
            /*...*/
            /*...*/
            /* END OF FILE USAGE */
            fclose(file);
        }
    }

    return 1;
}