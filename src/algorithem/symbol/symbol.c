#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "../../utils/utils.h"
#include "../../logger/logger.h"

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
