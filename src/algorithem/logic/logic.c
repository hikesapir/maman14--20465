#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "logic.h"
#include "../../utils/utils.h"
#include "../symbol/symbol.h"
#include "../command/command.h"

/* Decleration */

/**
 * @brief Converts arguments to binary representation based on their types and values.
 *
 * This function takes a list of commands and symbols, and for each command's arguments, it converts
 * them to binary representation based on their types and values. It handles registers, variables (labels
 * and externs), and static constants (numeric and string) accordingly.
 *
 * @param commands A pointer to the list of commands.
 * @param symbols A pointer to the list of symbols.
 */
void set_arguments_binary(Commands *, Symbols *);

/* Implementation */

void destructureFile(FILE *file, Symbols *symbols, Commands *commands)
{
    char line[LINE_LENGTH], *colon_ptr;          /* Buffer to store each line of the input file */
    int decimal_address = 100, line_in_file = 1; /* Starting address for commands */

    /* Definitions for various commands */
    CMD_Definition command_definition[] = {
        {"mov", MOV, 2},
        {"cmp", CMP, 2},
        {"add", ADD, 2},
        {"sub", SUB, 2},
        {"not", NOT, 1},
        {"clr", CLR, 1},
        {"lea", LEA, 2},
        {"inc", INC, 1},
        {"dec", DEC, 1},
        {"jmp", JMP, 1},
        {"bne", BNE, 1},
        {"red", RED, 1},
        {"prn", PRN, 1},
        {"jsr", JSR, 1},
        {"rts", RTS, 0},
        {"stop", STOP, 0},
        {".string", STRING, -1},
        {".data", DATA, -1}};

    /* Initialize the symbols and commands structures */
    symbols->array = (Symbol **)malloc(sizeof(Symbol *));
    symbols->amount = 0;

    commands->array = (Command **)malloc(sizeof(Command *));
    commands->amount = 0;

    /* Reset the file pointer to the beginning of the file */
    rewind(file);

    /* First scan: Get all externs, entries, labels, and commands */
    while (fgets(line, LINE_LENGTH, file))
    {
        /* If the line is empty */
        if (trim(line)[0] == '\0')
            continue;

        /* If the line contains ".extern" */
        else if (strstr(line, ".extern") != NULL)
            insertNewSymbol(line, symbols, strlen(".extern\0"), EXTERN, line_in_file);

        /* If the line contains ".entry" */
        else if (strstr(line, ".entry") != NULL)
            insertNewSymbol(line, symbols, strlen(".entry\0"), ENTRY, line_in_file);

        /* If the line contains a label (indicated by a colon) */
        else if (strstr(line, ":") != NULL)
        {
            /* Insert the new label symbol and the associated command */
            insertNewLabel(line, symbols, decimal_address, line_in_file);

            colon_ptr = strstr(line, ":");

            insertNewCommand(colon_ptr + 1, commands, &decimal_address, command_definition, line_in_file);
        }

        /* If the line contains an instruction or a command */
        else
            insertNewCommand(line, commands, &decimal_address, command_definition, line_in_file);

        line_in_file += 1;
    }

    /* Set binary representations for arguments */
    set_arguments_binary(commands, symbols);
}

void set_arguments_binary(Commands *commands, Symbols *symbols)
{
    int command_index;  /* Index for iterating through commands */
    int argument_index; /* Index for iterating through arguments */
    int symbols_index;  /* Index for iterating through symbols */
    int binary_index;   /* Index for iterating through binary representation */

    int static_variable_binary; /* Binary representation of static variable */
    int static_string_ascii;    /* ASCII value of the static string */
    int static_variable_value;  /* Value of the static variable */
    int binary_digit;           /* Individual binary digit */
    int binary_register_number; /* Binary representation of register number */
    int decimal_adress_binary;  /* Binary representation of decimal address */
    int divider;                /* Divider used for extracting digits */
    int register_number;        /* Extracted register number */

    Command *command;
    Argument *argument;
    Symbol *symbol, *running_symbol;

    /* Loop through each command */
    for (command_index = 0; command_index < commands->amount; command_index++)
    {
        command = commands->array[command_index];

        /* Skip invalid commands */
        if (command->command_type == INVALID_COMMAND)
            continue;

        /* Loop through each argument in the command */
        for (argument_index = 0; argument_index < command->arguments.amount; argument_index++)
        {
            argument = command->arguments.arr[argument_index];

            /* Handle different argument types */
            switch (argument->type)
            {
            case REGISTER:
                /* Initialize binary representation to 000...000 */
                for (binary_index = 0; binary_index < 12; binary_index++)
                    argument->binary_representation[binary_index] = 0;

                /* Extract register number from the argument name */
                register_number = atoi(argument->name + 2);
                binary_register_number = int_to_binary(register_number);

                /* Set binary representation based on argument index (destination or source) */
                if (argument_index == 0)
                    for (divider = 1, binary_index = 9; binary_index >= 5; binary_index--, divider *= 10)
                        argument->binary_representation[binary_index] = binary_register_number / divider % 10;
                else
                    for (divider = 1, binary_index = 4; binary_index >= 0; binary_index--, divider *= 10)
                        argument->binary_representation[binary_index] = binary_register_number / divider % 10;
                break;

            case VARIABLE:
                symbol = NULL;

                /* Find the symbol matching the argument name */
                for (symbols_index = 0; symbols_index < symbols->amount; symbols_index++)
                {
                    running_symbol = symbols->array[symbols_index];

                    if (strcmp(running_symbol->name, argument->name) == 0 &&
                        (running_symbol->type == LABEL || running_symbol->type == EXTERN))
                        symbol = running_symbol;
                }

                /* Symbol not found */
                if (symbol == NULL)
                {
                    command->command_type = INVALID_COMMAND;
                    break;
                }

                /* Handle symbol types */
                if (symbol->type == LABEL)
                {
                    /* Set binary representation for label addresses */
                    decimal_adress_binary = int_to_binary(symbol->decimal_address);
                    for (binary_index = 9, divider = 1; binary_index >= 0; binary_index--, divider *= 10)
                        argument->binary_representation[binary_index] = decimal_adress_binary / divider % 10;

                    argument->binary_representation[10] = 1;
                    argument->binary_representation[11] = 0;
                }

                if (symbol->type == EXTERN)
                {
                    /* Set binary representation for extern references */
                    for (binary_index = 0; binary_index < 11; binary_index++)
                        argument->binary_representation[binary_index] = 0;
                    argument->binary_representation[11] = 1;
                }

                break;

            case STATIC:
                /* Initialize bits 10 and 11 */
                argument->binary_representation[10] = 0;
                argument->binary_representation[11] = 0;
                static_variable_value = 0;

                /* Handle numeric and string constants */
                if (command->command_type == STRING)
                {
                    static_string_ascii = (argument->name[0]);
                    static_variable_binary = int_to_binary(static_string_ascii);
                }
                else
                {
                    static_variable_value = atoi(argument->name);

                    /* Calculate binary representation for negative numbers */
                    if (static_variable_value < 0)
                        static_variable_binary = int_to_binary(-static_variable_value - 1);
                    else
                        static_variable_binary = int_to_binary(static_variable_value);
                }

                /* Convert static variable binary representation */
                for (binary_index = 9, divider = 1; binary_index >= 0; binary_index--, divider *= 10)
                {
                    binary_digit = static_variable_binary / divider % 10;
                    if (static_variable_value < 0)
                        argument->binary_representation[binary_index] = 1 - binary_digit;
                    else
                        argument->binary_representation[binary_index] = binary_digit;
                }

                break;
            default:
                break;
            }
        }
    }
}
