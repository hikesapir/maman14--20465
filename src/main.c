#include <stdio.h>
#include <stdlib.h>
#include "validations/fileValidation.h"
#include "macros/macro.h"
#include "logger/logger.h"
#include "algorithem/logic/logic.h"
#include "algorithem/command/command.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *file;

    /* Run the script */
    if (argc == 1)
    {
        logError("No filename entered :(");
        exit(-1);
    }

    for (i = 1; i < argc; ++i)
    {
        file = validateFile(argv[i]);
        if (file != NULL)
        {
            /* START OF FILE USAGE */
            file = stripMacros(file, argv[i]);
            destructureFile(file);
            fclose(file);
        }
    }

    return 1;
}