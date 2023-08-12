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
            commands.amount = 0;
            symbols.amount = 0;

            /* START OF FILE USAGE */
            file = stripMacros(file, argv[i]);

            destructureFile(file, &symbols, &commands);

            fclose(file);

            /* 1. Do Not Create the files if there is INVALID command or symbol and return */
            if (has_invalid_command(commands) || has_invalid_symbol(symbols))
            {
                free_symbols(&symbols);
                free_commands(&commands);
                continue;
            }

            /* 2. For each command */
            /* header of base64 will be (amount of command lines, amount of variable lines) */
            /* write command from binary to base64 file */
            /* write to externs usage file */
            /* write the entries to the entries file */

            /* Free memory associated with Symbols and Commands */
            free_symbols(&symbols);
            free_commands(&commands);
        }
    }

    return 0;
}