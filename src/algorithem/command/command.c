#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "command.h"
#include "../../utils/utils.h"
#include "../../logger/logger.h"

/* Decleration */

/**
 * @brief Determine the command type based on the input line.
 *
 * This function identifies the command type based on the command name in the input line.
 * It searches for a match in the command_definition array and updates the command_type accordingly.
 *
 * @param command_type Pointer to store the detected command type.
 * @param line The input line to analyze.
 * @param command_definition An array of CMD_Definition representing command definitions.
 * @param line_in_file The line number in the input file where the command is located.
 * @return Pointer to the remaining part of the line after the command name,
 *         or NULL if the command is undefined.
 */
char *get_command_type(Command_Type *, char *, CMD_Definition[], int);
/**
 * @brief Parse the string command arguments from the input line.
 *
 * This function populates the Arguments structure for a STRING command.
 *
 * @param command Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 * @param line_in_file The line number in the input file where the command is located.
 */
void get_string_command_arguments(Command *, char *, int);
/**
 * @brief Parse and extract the arguments for a command.
 *
 * This function tokenizes the input line to separate individual arguments
 * and stores them in the Arguments structure. It determines the argument type
 * and allocates memory to store the argument name.
 *
 * @param arguments Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 */
void get_command_arguments(Arguments *, char *);
/**
 * @brief Check if command arguments are valid.
 *
 * This function checks if the given command's arguments are valid according to
 * the specified command definitions. It ensures the correct number of arguments
 * and their types match the expected format.
 *
 * @param command Pointer to the command structure.
 * @param command_definition An array of CMD_Definition representing command definitions.
 * @param line_in_file The line number in the input file where the command is located.
 * @return True if the arguments are valid, otherwise false.
 */
bool arguments_is_valid(Command *, CMD_Definition[], int);
/**
 * @brief Advance the decimal address based on the command's arguments.
 *
 * This function updates the decimal address based on the number and types of
 * arguments in the given command. It considers both registers and non-register
 * arguments to determine the appropriate address advancement.
 *
 * @param command Pointer to the command structure.
 * @param decimal_address A pointer to the decimal address value.
 */
void advance_decimal_adress(Command *, int *);
/**
 * @brief Set the binary representation for a command.
 *
 * This function sets the binary representation for the given command based on its type and arguments.
 *
 * @param command Pointer to the command structure.
 */
void set_command_binary(Command *);

/* Implementation */

void insertNewCommand(char *line, Commands *commands, int *decimal_address, CMD_Definition command_definition[], int line_in_file)
{
    Command_Type command_type = INVALID_COMMAND;
    Command *new_command;

    /* add new command */
    commands->amount++;
    commands->array = (Command **)realloc(commands->array, commands->amount * sizeof(Command *));
    commands->array[commands->amount - 1] = (Command *)malloc(sizeof(Command));
    new_command = commands->array[commands->amount - 1];

    /* Allocate memory for the arguments array */
    new_command->arguments.arr = (Argument **)malloc(sizeof(Argument *));
    new_command->arguments.amount = 0;

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    /* Identify the command type and set it in the new_command */
    line = get_command_type(&command_type, line, command_definition, line_in_file);
    new_command->command_type = command_type;

    if (command_type == INVALID_COMMAND)
        return;

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    new_command->arguments.arr = NULL;
    if (command_type == STRING)
        get_string_command_arguments(new_command, line, line_in_file);
    else
        get_command_arguments(&new_command->arguments, line);

    /* command_type can change due to invalid string */
    if (new_command->command_type == INVALID_COMMAND)
        return;

    if (!arguments_is_valid(new_command, command_definition, line_in_file))
    {
        new_command->command_type = INVALID_COMMAND;
        return;
    }

    new_command->decimal_address = *decimal_address;
    advance_decimal_adress(new_command, decimal_address);

    /* Set the binary representation for the command */
    set_command_binary(new_command);
}

char *get_command_type(Command_Type *command_type, char *line, CMD_Definition command_definition[], int line_in_file)
{
    int i = 0, length = 0;
    char *command_name;

    /* Find the length of the command name by scanning until whitespace or end of string */
    while (!isspace(line[i]) && line[i] != '\0')
        i++;
    length = i;

    /* Allocate memory for the command name and copy it */
    command_name = (char *)malloc(sizeof(char) * (length + 1));
    strncpy(command_name, line, length);
    command_name[length] = '\0';

    /* Loop through command definitions to find a match */
    for (i = 0; i < CMD_DEFINITIONS_AMOUNT; i++)
        if (strcmp(command_definition[i].name, command_name) == 0)
        {
            *command_type = command_definition[i].type;
            free(command_name);   /* Free allocated memory */
            return line + length; /* Return pointer to the remaining part of the line */
        }

    /* Log an error if the command is not found in the definitions */
    logError("line %d: Undefined Command: %s", line_in_file, command_name);

    *command_type = INVALID_COMMAND;
    free(command_name); /* Free allocated memory */
    return NULL;
}

void get_string_command_arguments(Command *command, char *line, int line_in_file)
{
    int i;
    Argument *new_argument;
    Arguments *arguments = &command->arguments;
    char argument;

    if (*line == '\0')
        return;

    /* If the input match the expected format */
    if (*line == '"' && *(line + strlen(line) - 1) == '"')
    {
        for (i = 1; i < strlen(line) - 1; i++)
        {
            argument = line[i];

            arguments->amount += 1;
            arguments->arr = (Argument **)realloc(arguments->arr, arguments->amount * sizeof(Argument *));
            arguments->arr[arguments->amount - 1] = (Argument *)malloc(sizeof(Argument));
            new_argument = arguments->arr[arguments->amount - 1];

            /* Extract and store the string argument (without quotes) */

            new_argument->name = (char *)malloc(2 * sizeof(char));
            new_argument->name[0] = argument;
            new_argument->name[1] = '\0';
            new_argument->type = STATIC;
            new_argument->decimal_address = 0;
        }

        /* Extract and store the \0 argument */
        arguments->amount += 1;
        arguments->arr = (Argument **)realloc(arguments->arr, arguments->amount * sizeof(Argument *));
        arguments->arr[arguments->amount - 1] = (Argument *)malloc(sizeof(Argument));
        new_argument = arguments->arr[arguments->amount - 1];

        new_argument->name = (char *)malloc(1 * sizeof(char));
        new_argument->name[0] = '\0';
        new_argument->type = STATIC;
        new_argument->decimal_address = 0;
    }
    else
    {
        logError("line %d: .string content is not wraped with quotes (\"...\")", line_in_file);
        command->command_type = INVALID_COMMAND;
    }
}

void get_command_arguments(Arguments *arguments, char *line)
{
    char *token = strtok(line, ARGUMENTS_DELIMITER); /* Tokenize the input line */
    Argument *new_argument;

    if (token == NULL)
        return;

    /* walk through arguments */
    while (token != NULL)
    {
        token = trim(token); /* Trim whitespace from the token */

        arguments->amount++;
        arguments->arr = (Argument **)realloc(arguments->arr, arguments->amount * sizeof(Argument *));
        arguments->arr[arguments->amount - 1] = (Argument *)malloc(sizeof(Argument));
        new_argument = arguments->arr[arguments->amount - 1];

        new_argument->name = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(new_argument->name, token);
        new_argument->name[strlen(token)] = '\0';

        /* Determine the argument type based on the token content */
        if (*token == '@' && *(token + 1) == 'r' && (token[2] - '0') >= 0 && (token[2] - '0') <= 7)
            new_argument->type = REGISTER;
        else if (is_numeric(token) == true)
            new_argument->type = STATIC;
        else
            new_argument->type = VARIABLE;

        token = strtok(NULL, ARGUMENTS_DELIMITER); /* Move to the next token */
    }
}

bool arguments_is_valid(Command *command, CMD_Definition command_definition[], int line_in_file)
{
    int i;
    Arguments arguments = command->arguments;
    CMD_Definition cmd_definition;

    for (i = 0; i < CMD_DEFINITIONS_AMOUNT; i++)
        if (command_definition[i].type == command->command_type)
            cmd_definition = command_definition[i];

    if (cmd_definition.args_count != arguments.amount && cmd_definition.args_count != -1)
    {
        logError("line %d: Unexpected amount of arguments for command %s (expected:%d recieved:%d)", line_in_file, cmd_definition.name, cmd_definition.args_count, arguments.amount);
        return false;
    }

    switch (cmd_definition.type)
    {
    case CMP:    /* TWO ARGUMENT ALL TYPES */
    case PRN:    /* ONE ARGUMENT ALL TYPES */
    case STRING: /* ARGUMENTS IS STATIC */
    case DATA:   /* ARGUMENTS IS STATIC */
    case RTS:    /* NO ARGUMENTS */
    case STOP:   /* NO ARGUMENTS */
        return true;
    case LEA: /* TWO ARGUMENT FIRST - ALL BUT STATIC, SECOND IS VARIABLE */
        if (arguments.arr[1]->type != VARIABLE)
        {
            logError("line %d: Unexpected type of second argument for command lea", line_in_file);
            return false;
        }
    default: /* FIRST ARGUMENT ALL BUT STATIC */
        if (arguments.arr[0]->type == STATIC)
        {
            logError("line %d: Unexpected type of first argument for command %s", line_in_file, cmd_definition.name);
            return false;
        }
        else
            return true;
    }
}

void advance_decimal_adress(Command *command, int *decimal_address)
{
    int arg_index;
    bool both_registers =
        (command->arguments.amount == 2 &&
         command->arguments.arr[0]->type == REGISTER &&
         command->arguments.arr[1]->type == REGISTER);

    if (both_registers)
    {
        *decimal_address += 1;

        command->arguments.arr[0]->decimal_address = *decimal_address;
        command->arguments.arr[1]->decimal_address = *decimal_address;
    }
    else
        for (arg_index = 0; arg_index < command->arguments.amount; arg_index++)
            if (command->command_type == DATA || command->command_type == STRING)
            {
                command->arguments.arr[arg_index]->decimal_address = *decimal_address;
                *decimal_address += 1;
            }
            else
            {
                *decimal_address += 1;
                command->arguments.arr[arg_index]->decimal_address = *decimal_address;
            }

    if (command->command_type != DATA && command->command_type != STRING)
        *decimal_address += 1; /* For the line of the next command */
}

bool has_invalid_command(Commands commands)
{
    int i;
    for (i = 0; i < commands.amount; i++)
        if (commands.array[i]->command_type == INVALID_COMMAND)
            return true;
    return false;
}

void free_commands(Commands *commands)
{
    int i, j;
    Command *command;
    Argument *argument;
    /* Free memory for each individual Command structure */
    for (i = 0; i < commands->amount; i++)
    {
        command = commands->array[i];

        /* Free the memory for each argument inside the Command */
        for (j = 0; j < command->arguments.amount; j++)
        {
            argument = command->arguments.arr[j];
            free(argument->name);
            free(argument);
        }
        if (command->arguments.arr != NULL)
            free(command->arguments.arr);

        /* Free the memory for the Command structure itself */
        free(command);
    }
    /* Free memory for the array of Command pointers */
    free(commands->array);
}

void set_command_binary(Command *command)
{
    int command_type_binary, argument_type_binary;

    if (command->command_type == STRING || command->command_type == DATA)
        return;

    /* A,R,E */
    command->binary_representation[10] = 0;
    command->binary_representation[11] = 0;

    /* Command */
    command_type_binary = int_to_binary((int)command->command_type);
    command->binary_representation[3] = command_type_binary / 1000 % 10;
    command->binary_representation[4] = command_type_binary / 100 % 10;
    command->binary_representation[5] = command_type_binary / 10 % 10;
    command->binary_representation[6] = command_type_binary % 10;

    /* Source */
    command->binary_representation[0] = 0;
    command->binary_representation[1] = 0;
    command->binary_representation[2] = 0;

    /* Destination */
    command->binary_representation[7] = 0;
    command->binary_representation[8] = 0;
    command->binary_representation[9] = 0;

    if (command->arguments.amount == 1)
    {
        /* Destination */
        argument_type_binary = int_to_binary((int)command->arguments.arr[0]->type);
        command->binary_representation[7] = argument_type_binary / 100 % 10;
        command->binary_representation[8] = argument_type_binary / 10 % 10;
        command->binary_representation[9] = argument_type_binary % 10;
    }

    if (command->arguments.amount == 2)
    {
        /* Source */
        argument_type_binary = int_to_binary((int)command->arguments.arr[0]->type);
        command->binary_representation[0] = argument_type_binary / 100 % 10;
        command->binary_representation[1] = argument_type_binary / 10 % 10;
        command->binary_representation[2] = argument_type_binary % 10;

        /* Destination */
        argument_type_binary = int_to_binary((int)command->arguments.arr[1]->type);
        command->binary_representation[7] = argument_type_binary / 100 % 10;
        command->binary_representation[8] = argument_type_binary / 10 % 10;
        command->binary_representation[9] = argument_type_binary % 10;
    }
}
