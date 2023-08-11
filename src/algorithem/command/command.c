#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "command.h"
#include "../../utils/utils.h"
#include "../../logger/logger.h"

void print_arguments(Arguments arguments)
{
    Argument *argument;
    int i = 0;
    for (i = 0; i < arguments.amount; i++)
    {
        argument = arguments.arr[i];
        printf("%s %d\n", argument->name, argument->type);
    }
}

void insertNewCommand(char *line, Commands *commands, int *decimal_address, CMD_Definition command_definition[])
{
    Command_Type command_type = INVALID;
    Arguments arguments;
    Command *new_command;

    arguments.amount = 0;

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    line = get_command_type(&command_type, line, command_definition);

    if (command_type == INVALID)
        return;

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    /* add new command*/
    commands->amount++;
    commands->array = (Command **)realloc(commands->array, commands->amount * sizeof(Command *));
    commands->array[commands->amount - 1] = malloc(sizeof(Command));
    new_command = commands->array[commands->amount - 1];

    if (command_type == STRING)
        get_string_command_arguments(new_command, line);
    else
        get_command_arguments(&arguments, line);

    new_command->command_type = command_type;
    new_command->arguments = arguments;
    new_command->decimal_address = *decimal_address;

    /* advance_decimal_adress(new_command, decimal_address); */

    *decimal_address += 1;
}

char *get_command_type(Command_Type *command_type, char *line, CMD_Definition command_definition[])
{
    int i = 0, length = 0;
    char *command_name;

    /* Find the length of the command name by scanning until whitespace or end of string */
    while (!isspace(line[length]) && line[length] != '\0')
        length++;

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

    if (*line == '"' && *(line + strlen(line) - 1) == '"')
    {
        /* Allocate memory for the arguments array */
        arguments->arr = (Argument **)malloc(sizeof(Argument *));
        arguments->amount = 1;
        arguments->arr[arguments->amount - 1] = malloc(sizeof(Argument));
        new_argument = arguments->arr[arguments->amount - 1];
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