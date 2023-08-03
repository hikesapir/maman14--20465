#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utils/utils.h"
#include "symbol.h"
#include "command.h"
#include "../logger/logger.h"
#include <stdbool.h>

/**
 * Inserts a new symbol into the Symbols structure based on the given line and type.
 *
 * This function extracts the symbol name from the provided line and inserts it into
 * the Symbols structure with the given symbol type. It first trims leading and trailing
 * spaces from the line and then processes the line to extract the symbol name before
 * adding it to the Symbols structure.
 *
 * @param line The line containing the symbol declaration (e.g., ".extern symbol_name").
 * @param symbols A pointer to the Symbols structure to which the new symbol will be added.
 * @param symbol_length The length of the symbol type declaration (e.g., strlen(".extern\0")).
 * @param type The type of the new symbol to be added.
 */
void insertNewSymbol(char *, Symbols *, size_t, SYMBOL_TYPE);
/**
 * Inserts a new command into the Commands structure based on the given line and address.
 *
 * This function processes the given line to extract and parse the command information.
 * It then adds the parsed command to the Commands structure with the provided address.
 *
 * @param line The line containing the command information to be parsed and added.
 * @param commands The Commands structure to which the new command will be added.
 * @param decimal_address The decimal address associated with the command.
 */
void insertNewCommand(char *, Commands, int *);
/**
 * Inserts a new label symbol into the Symbols structure with the given address.
 *
 * This function extracts the label name from the provided line and inserts it into
 * the Symbols structure with the LABEL type and the given decimal address. It trims
 * leading and trailing spaces from the line and processes it to extract the label name.
 *
 * @param line The line containing the label declaration (e.g., "label_name:").
 * @param symbols A pointer to the Symbols structure to which the new label symbol will be added.
 * @param decimal_address The decimal address associated with the label.
 */
void insertNewLabel(char *, Symbols *, int);
/**
 * Adds a new symbol to the Symbols structure if it is valid.
 *
 * This function attempts to add a new symbol to the Symbols structure. Before adding,
 * it calls the newSymbolIsValid() function to check if the symbol is valid and doesn't
 * violate any rules (e.g., duplicate name, spaces in the name). If the symbol is valid,
 * it is added to the structure.
 *
 * @param symbols A pointer to the Symbols structure to which the new symbol will be added.
 * @param name The name of the new symbol to be added.
 * @param type The type of the new symbol to be added.
 */
void addNewSymbol(Symbols *, char *, SYMBOL_TYPE, int);
/**
 * Checks if a new symbol is valid and can be added to the Symbols structure.
 *
 * This function checks if the provided symbol name is valid for insertion into the Symbols
 * structure. It verifies that the symbol name doesn't already exist in the structure with
 * the same type (except for LABEL type, which allows duplicates). It also ensures that the
 * symbol name doesn't contain spaces.
 *
 * @param symbols A pointer to the Symbols structure containing existing symbols.
 * @param name The name of the new symbol to be checked for validity.
 * @param type The type of the new symbol to be checked for validity.
 * @return Returns true if the new symbol is valid, false otherwise.
 */
bool newSymbolIsValid(Symbols *, char *, SYMBOL_TYPE);

Command_Type get_command_type(char *);

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

            insertNewCommand(colon_ptr + 1, commands, &decimal_address);
        }

        /* If the line contains an instruction or a command */
        else
            insertNewCommand(line, commands, &decimal_address);
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

bool newSymbolIsValid(Symbols *symbols, char *name, SYMBOL_TYPE type)
{
    int i;
    Symbol *symbol;
    bool name_exists, same_type, has_spaces, both_not_labels;

    for (i = 0; i < symbols->amount; i++)
    {
        symbol = symbols->array[i];
        name_exists = strcmp(symbol->name, name) == 0;
        same_type = symbol->type == type;
        has_spaces = strstr(name, " ") != NULL;
        both_not_labels = type != LABEL && symbol->type != LABEL;

        /* Check for duplicate names with the same type (or same type except LABEL type) in the Symbols structure */
        if (name_exists && (both_not_labels || same_type))
        {
            logError("Duplicat symbols '%s' found", name);
            return false;
        }

        /* Check for names with spaces, which are invalid for symbols */
        if (has_spaces)
        {
            logError("Name '%s' has spaces", name);
            return false;
        }
    }

    return true;
}

void addNewSymbol(Symbols *symbols, char *name, SYMBOL_TYPE type, int decimal_address)
{
    Symbol *new_symbol = NULL;

    /* Check if the new symbol is valid before adding */
    if (newSymbolIsValid(symbols, name, type))
    {
        /* Increment the amount of symbols */
        symbols->amount += 1;

        /* Reallocate memory for the Symbols array to accommodate the new symbol */
        symbols->array = (Symbol **)realloc(symbols->array, symbols->amount * sizeof(Symbol *));

        /* Allocate memory for the new symbol and add it to the Symbols array */
        symbols->array[symbols->amount - 1] = malloc(sizeof(Symbol));
        new_symbol = symbols->array[symbols->amount - 1];

        /* Allocate memory for the new symbol name and set its properties */
        new_symbol->name = malloc((strlen(name) + 1) * sizeof(char));
        strcpy(new_symbol->name, name);
        new_symbol->decimal_address = 0;
        new_symbol->type = type;
        new_symbol->decimal_address = decimal_address;
    }
}

void insertNewSymbol(char *line, Symbols *symbols, size_t symbol_length, SYMBOL_TYPE type)
{
    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    /* Slice the string starting from 'symbol_length' */
    line = slice(line, symbol_length);

    /* Trim leading and trailing spaces from the sliced string */
    line = trim(line);

    /* Add the new symbol to the Symbols structure */
    addNewSymbol(symbols, line, type, 0);
}

void insertNewLabel(char *line, Symbols *symbols, int decimal_address)
{
    char *label = NULL, *colon_ptr = NULL;
    int size_of_label = 0;

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    /* Find the position of the colon (:) in the line to determine the label's size */
    colon_ptr = strstr(line, ":");

    /* Calculate the size of the label (number of characters before the colon) */
    size_of_label = colon_ptr - line;

    /* Allocate memory for the label and copy its name from 'line' */
    label = malloc(sizeof(char) * (size_of_label + 1));
    memcpy(label, line, size_of_label);
    label[size_of_label] = '\0';

    /* Add the new label symbol to the Symbols structure */
    addNewSymbol(symbols, label, LABEL, decimal_address);
}

void insertNewCommand(char *line, Commands commands, int *decimal_address)
{
    Command_Type ctype = MOV;

    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    ctype = get_command_type(line);
}

Command_Type get_command_type(char *line)
{
    int i = 0, length = 0, amount_of_command_defs = 0;
    char *command_name;

    /* run until reached white space or end of the string*/
    while (!isspace(line[length]) && line[length] != '\0')
        length++;

    command_name = malloc(sizeof(char) * (length + 1));
    strncpy(command_name, line, length);
    command_name[length] = '\0';

    amount_of_command_defs = sizeof(command_definition) / sizeof(CMD_Definition);
    for (i = 0; i < amount_of_command_defs; i++)
        if (strcmp(command_definition[i].name, command_name) == 0)
            return command_definition[i].type;

    logError("Undefined Command: %s", command_name);
    return INVALID;
}