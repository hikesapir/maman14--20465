#include <stdio.h>
#include <stdlib.h>
#include "validations/fileValidation.h"
#include "macros/macro.h"
#include "logger/logger.h"
#include "algorithem/logic/logic.h"
#include "algorithem/command/command.h"
#include "algorithem/symbol/symbol.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *file;
    Symbols symbols;   /* Structure to store different symbols found in the file */
    Commands commands; /* Structure to store the parsed commands from the file */

    /* Check if filename is provided */
    if (argc == 1)
    {
        logError("No filename entered :(");
        exit(-1);
    }

    /* Process each provided filename */
    for (i = 1; i < argc; ++i)
    {
        /* Validate the file */
        file = validateFile(argv[i]);
        if (file != NULL)
        {
            commands.amount = 0;
            symbols.amount = 0;

            /* Remove macros and process the file */
            file = stripMacros(file, argv[i]);

            /* Extract symbols and commands from the file */
            destructureFile(file, &symbols, &commands);

            fclose(file);

            /* Check for invalid commands or symbols */
            if (has_invalid_command(commands) || has_invalid_symbol(symbols))
            {
                /* Clean up and skip to the next file */
                free_symbols(&symbols);
                free_commands(&commands);
                continue;
            }

            /* TODO: Process each command */

            /* write the entries to the entries file */

            /* write command from binary to base64 file */
            /* header of base64 will be (amount of command lines, amount of variable lines) */

            /* write to externs usage file */

            /* Free memory associated with Symbols and Commands */
            free_symbols(&symbols);
            free_commands(&commands);
        }
    }

    return 0;
}