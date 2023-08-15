#include <stdio.h>
#include <string.h>

#include "../../utils/utils.h"
#include "../symbol/symbol.h"
#include "../command/command.h"
#include "../../logger/logger.h"

void write_entries(Symbols, char *);
void write_externs(Commands, Symbols, char *);
void binary_to_base64(Commands, Symbols, char *);

void write_files(Commands commands, Symbols symbols, char *file_name)
{
    write_entries(symbols, file_name);
    write_externs(commands, symbols, file_name);
    binary_to_base64(commands, symbols, file_name);
}

void write_entries(Symbols symbols, char *file_name)
{
    int entry_index, label_index;
    Symbol *label, *entry;
    FILE *entries_file = create_output_file(file_name, ".ent");

    if (entries_file == NULL)
    {
        logError("Could not create file %s.ent", file_name);
        return;
    }
    else
        logInfo("File opened successfully!");

    for (label_index = 0; label_index < symbols.amount; label_index++)
        if (symbols.array[label_index]->type == LABEL)
        {
            label = symbols.array[label_index];
            for (entry_index = 0; entry_index < symbols.amount; entry_index++)
                if (symbols.array[entry_index]->type == ENTRY)
                {
                    entry = symbols.array[entry_index];
                    if (strcmp(entry->name, label->name) == 0)
                        fprintf(entries_file, "%s\t%d\n", entry->name, label->decimal_address);
                }
        }
}

void write_externs(Commands commands, Symbols symbols, char *file_name)
{
    int command_index, argument_index, symbol_index;
    FILE *extern_file = create_output_file(file_name, ".ext");
    Command *command;
    Argument *argument;
    Symbol *symbol;

    if (extern_file == NULL)
    {
        logError("Could not create file %s.ext", file_name);
        return;
    }
    else
        logInfo("File opened successfully!");

    for (command_index = 0; command_index < commands.amount; command_index++)
    {
        command = commands.array[command_index];
        for (argument_index = 0; argument_index < command->arguments.amount; argument_index++)
        {
            argument = command->arguments.arr[argument_index];
            if (argument->type == VARIABLE)
                for (symbol_index = 0; symbol_index < symbols.amount; symbol_index++)
                {
                    symbol = symbols.array[symbol_index];
                    if (symbol->type == EXTERN && strcmp(symbol->name, argument->name) == 0)
                        fprintf(extern_file, "%s\t%d\n", symbol->name, argument->decimal_address);
                }
        }
    }
}

void binary_to_base64(Commands commands, Symbols symbols, char *file_name)
{
    FILE *extern_file = create_output_file(file_name, ".ent");

    if (extern_file == NULL)
    {
        logError("Could not create file %s.ent", file_name);
        return;
    }
    else
        logInfo("File opened successfully!");

    /* write command from binary to base64 file */
    /* header of base64 will be (amount of command lines, amount of variable lines) */

    /* write to externs usage file */
}
