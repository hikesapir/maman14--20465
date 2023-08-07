#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "logic.h"
#include "../../utils/utils.h"
#include "../symbol/symbol.h"
#include "../command/command.h"

void print_command(Command command)
{
    printf("**** new command ****\n command type is: %d place in address: %d\n paramas:", command.command_type, command.decimal_address);
    print_arguments(command.arguments);
}

Commands destructureFile(FILE *file)
{
    char line[LINE_LENGTH], *colon_ptr; /* Buffer to store each line of the input file */
    int decimal_address = 100;          /* Starting address for commands */
    Symbols symbols;                    /* Structure to store different symbols found in the file */
    Commands commands;                  /* Structure to store the parsed commands from the file */

    /* Initialize the symbols and commands structures */
    symbols.array = (Symbol **)malloc(sizeof(Symbol *));
    symbols.amount = 0;

    commands.array = (Command **)malloc(sizeof(Command *));
    commands.amount = 0;

    /* Reset the file pointer to the beginning of the file */
    rewind(file);

    /* First scan: Get all externs, entries, labels, and commands */
    while (fgets(line, LINE_LENGTH, file))
    {
        /* If the line contains ".extern" */
        if (strstr(line, ".extern") != NULL)
            insertNewSymbol(line, &symbols, strlen(".extern\0"), EXTERN);

        /* If the line contains ".entry" */
        else if (strstr(line, ".entry") != NULL)
            insertNewSymbol(line, &symbols, strlen(".entry\0"), ENTRY);

        /* If the line contains a label (indicated by a colon) */
        else if (strstr(line, ":") != NULL)
        {
            /* Insert the new label symbol and the associated command */
            insertNewLabel(line, &symbols, decimal_address);

            colon_ptr = strstr(line, ":");

            insertNewCommand(colon_ptr + 1, &commands, &decimal_address);
            print_command(*commands.array[commands.amount - 1]);
        }

        /* If the line contains an instruction or a command */
        else
        {
            insertNewCommand(line, &commands, &decimal_address);

            print_command(*commands.array[commands.amount - 1]);
        }
    }

    /* Reset the file pointer to the beginning of the file */
    rewind(file);

    /* second scan (commands.array): place externs and entries to files and commands unknown binary */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */

    return commands;
}
