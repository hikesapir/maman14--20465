#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "command.h"
#include "../../utils/utils.h"
#include "../../logger/logger.h"

void insertNewCommand(char *line, Commands *commands, int *decimal_address, CMD_Definition command_definition[])
{
    Command_Type command_type = INVALID;
    Command *new_command;

    /* add new command */
    commands->amount++;
    commands->array = (Command **)realloc(commands->array, commands->amount * sizeof(Command *));
    commands->array[commands->amount - 1] = malloc(sizeof(Command));
    new_command = commands->array[commands->amount - 1];

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    /* Identify the command type and set it in the new_command */
    line = get_command_type(&command_type, line, command_definition);
    new_command->command_type = command_type;

    if (command_type == INVALID)
        return;

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    if (command_type == STRING)
        get_string_command_arguments(new_command, line);
    else
        get_command_arguments(&new_command->arguments, line);

    /* command_type can change due to invalid string */
    if (new_command->command_type == INVALID)
        return;

    if (!arguments_is_valid(new_command, command_definition))
    {
        new_command->command_type = INVALID;
        return;
    }

    new_command->decimal_address = *decimal_address;
    advance_decimal_adress(new_command, decimal_address);
}

char *get_command_type(Command_Type *command_type, char *line, CMD_Definition command_definition[])
{
    int i = 0, length = 0;
    char *command_name;

    /* Find the length of the command name by scanning until whitespace or end of string */
    while (!isspace(line[i]) && line[i] != '\0')
        i++;
    length = i;

    /* Allocate memory for the command name and copy it */
    command_name = malloc(sizeof(char) * (length + 1));
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
    logError("Undefined Command: %s", command_name);

    *command_type = INVALID;
    free(command_name); /* Free allocated memory */
    return NULL;
}

void get_string_command_arguments(Command *command, char *line)
{
    Argument *new_argument;
    Arguments *arguments;

    arguments = &command->arguments;

    if (*line == '\0')
        return;

    /* If the input match the expected format */
    if (*line == '"' && *(line + strlen(line) - 1) == '"')
    {
        /* Allocate memory for the arguments array */
        arguments->arr = (Argument **)malloc(sizeof(Argument *));
        arguments->amount = 1;
        arguments->arr[arguments->amount - 1] = malloc(sizeof(Argument));
        new_argument = arguments->arr[arguments->amount - 1];

        /* Extract and store the string argument (without quotes) */
        new_argument->name = malloc((strlen(line) - 1) * sizeof(char));
        strncpy(new_argument->name, (line + 1), (strlen(line) - 2));
        new_argument->name[strlen(line) - 2] = '\0';
    }
    else
        command->command_type = INVALID;
}

void get_command_arguments(Arguments *arguments, char *line)
{
    char *token = strtok(line, ARGUMENTS_DELIMITER); /* Tokenize the input line */
    Argument *new_argument;

    arguments->amount = 0;

    if (token == NULL)
        return;

    /* Allocate memory for the arguments array */
    arguments->arr = (Argument **)malloc(sizeof(Argument *));

    /* walk through arguments */
    while (token != NULL)
    {
        token = trim(token); /* Trim whitespace from the token */

        arguments->amount++;
        arguments->arr = (Argument **)realloc(arguments->arr, arguments->amount * sizeof(Argument *));
        arguments->arr[arguments->amount - 1] = malloc(sizeof(Argument));
        new_argument = arguments->arr[arguments->amount - 1];

        new_argument->name = malloc((strlen(token) + 1) * sizeof(char));
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

bool arguments_is_valid(Command *command, CMD_Definition command_definition[])
{
    int i;
    Arguments arguments = command->arguments;
    CMD_Definition cmd_definition;

    for (i = 0; i < CMD_DEFINITIONS_AMOUNT; i++)
        if (command_definition[i].type == command->command_type)
            cmd_definition = command_definition[i];

    if (cmd_definition.args_count != arguments.amount && cmd_definition.args_count != -1)
    {
        logError("Unexpected amount of arguments for command %s (expected:%d recieved:%d)", cmd_definition.name, cmd_definition.args_count, arguments.amount);
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
            logError("Unexpected type of second argument for command lea");
            return false;
        }
    default: /* FIRST ARGUMENT ALL BUT STATIC */
        if (arguments.arr[0]->type == STATIC)
        {
            logError("Unexpected type of first argument for command %s", cmd_definition.name);
            return false;
        }
        else
            return true;
    }
}

void advance_decimal_adress(Command *command, int *decimal_address)
{
    bool both_registers =
        (command->arguments.amount == 2 &&
         command->arguments.arr[0]->type == REGISTER &&
         command->arguments.arr[1]->type == REGISTER);

    if (both_registers)
        *decimal_address += 1;
    else if (command->command_type == STRING)
        *decimal_address += strlen(command->arguments.arr[0]->name);
    else
        *decimal_address += command->arguments.amount;

    if (command->command_type != DATA)
        *decimal_address += 1; /* For the line of the next command */
}
