#include <stdio.h>
#include <stdlib.h>
#include "validations/fileValidation.h"
#include "macros/macro.h"
#include "logger/logger.h"
#include "algorithem/algorithem.h"
#include "algorithem/instruction.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *file;
    DestructuredFile destructuredFile;

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
            /* replace macros with their contents */
            file = stripMacros(file, argv[i]);

            /* first scan: get all externs, entries and instructions lists */
            destructuredFile = destructureFile(file);

            /* second scan (destructuredFile): place externs and entries to files and instructions unknown binary */

            /*...*/
            /*...*/
            /*...*/
            /* END OF FILE USAGE */
            fclose(file);
        }
    }

    return 1;
}