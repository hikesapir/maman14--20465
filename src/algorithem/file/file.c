#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../utils/utils.h"
#include "../symbol/symbol.h"
#include "../command/command.h"
#include "../../logger/logger.h"

/**
 * @brief Write symbol entries to an output file.
 *
 * This function writes the symbol entries to an output file with the ".ent" extension.
 *
 * @param symbols A Symbols structure containing symbol information.
 * @param file_name The base name of the output file.
 */
void write_entries(Symbols, char *);

/**
 * @brief Write external symbols to an output file.
 *
 * This function writes external symbols and their references to an output file with the ".ext" extension.
 *
 * @param commands A Commands structure containing command information.
 * @param symbols A Symbols structure containing symbol information.
 * @param file_name The base name of the output file.
 */
void write_externs(Commands, Symbols, char *);

/**
 * @brief Write base64-encoded commands and static data to an output file.
 *
 * This function writes base64-encoded commands and static data to an output file with the ".ob" extension.
 *
 * @param commands A Commands structure containing command information.
 * @param file_name The base name of the output file.
 */
void write_base64(Commands, char *);

/**
 * @brief Convert binary to base64 representation.
 *
 * This function converts a binary array to its base64 representation and stores it in the provided base64 array.
 *
 * @param base64 An array to store the resulting base64 characters (at least 3 characters in size).
 * @param binaryArray The binary representation array (12 bits).
 */
void binary_to_base64(char[3], const int[12]);

void write_files(Commands commands, Symbols symbols, char *file_name)
{
    write_entries(symbols, file_name);
    write_externs(commands, symbols, file_name);
    write_base64(commands, file_name);
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

void write_base64(Commands commands, char *file_name)
{
    FILE *base64_file = create_output_file(file_name, ".ob");
    Command *command;
    Argument *argument;
    int i, command_index, argument_index, static_rows = 0, command_rows = 0;
    bool both_registers;
    char **base64_commands, **base64_static;

    if (base64_file == NULL)
    {
        logError("Could not create file %s.ob", file_name);
        return;
    }
    else
        logInfo("File opened successfully!");

    base64_commands = (char **)malloc(sizeof(char *));
    base64_static = (char **)malloc(sizeof(char *));

    for (command_index = 0; command_index < commands.amount; command_index++)
    {
        command = commands.array[command_index];

        if (command->command_type == DATA || command->command_type == STRING)
            for (argument_index = 0; argument_index < command->arguments.amount; argument_index++)
            {
                argument = command->arguments.arr[argument_index];

                static_rows += 1;
                base64_static = (char **)realloc(base64_static, static_rows * sizeof(char *));
                base64_static[static_rows - 1] = (char *)malloc(3 * sizeof(char));

                binary_to_base64(base64_static[static_rows - 1], argument->binary_representation);
            }
        else
        {
            /* for the command binary representation */
            command_rows += 1;
            base64_commands = (char **)realloc(base64_commands, command_rows * sizeof(char *));
            base64_commands[command_rows - 1] = (char *)malloc(3 * sizeof(char));
            binary_to_base64(base64_commands[command_rows - 1], command->binary_representation);

            both_registers =
                (command->arguments.amount == 2 &&
                 command->arguments.arr[0]->type == REGISTER &&
                 command->arguments.arr[1]->type == REGISTER);

            if (both_registers)
            {
                command_rows += 1;
                for (i = 9; i >= 5; i--)
                    command->arguments.arr[0]->binary_representation[i] = command->arguments.arr[1]->binary_representation[i];
                base64_commands = (char **)realloc(base64_commands, command_rows * sizeof(char *));
                base64_commands[command_rows - 1] = (char *)malloc(3 * sizeof(char));
                binary_to_base64(base64_commands[command_rows - 1], command->arguments.arr[0]->binary_representation);
            }
            else
                for (argument_index = 0; argument_index < command->arguments.amount; argument_index++)
                {
                    argument = command->arguments.arr[argument_index];

                    command_rows += 1;
                    base64_commands = (char **)realloc(base64_commands, command_rows * sizeof(char *));
                    base64_commands[command_rows - 1] = (char *)malloc(3 * sizeof(char));
                    binary_to_base64(base64_commands[command_rows - 1], argument->binary_representation);
                }
        }
    }

    fprintf(base64_file, "%d %d", command_rows, static_rows);

    for (i = 0; i < command_rows; i++)
    {
        fprintf(base64_file, "\n%s", base64_commands[i]);
        free(base64_commands[i]);
    }
    free(base64_commands);

    for (i = 0; i < static_rows; i++)
    {
        fprintf(base64_file, "\n%s", base64_static[i]);
        free(base64_static[i]);
    }
    free(base64_static);

    fclose(base64_file);
}

void binary_to_base64(char base64[3], const int binaryArray[12])
{
    int bit_index, first_character_index = 0, second_character_index = 0;
    char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    /* bit 0-5 = first charachter */
    for (bit_index = 5; bit_index >= 0; bit_index--)
        first_character_index += binaryArray[bit_index] * pow(2, 5 - bit_index);
    base64[0] = base64_chars[first_character_index];

    /* bit 6-11 = last charachter */
    for (bit_index = 11; bit_index >= 6; bit_index--)
        second_character_index += binaryArray[bit_index] * pow(2, 11 - bit_index);
    base64[1] = base64_chars[second_character_index];

    base64[2] = '\0';
}
