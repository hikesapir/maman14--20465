#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "../../utils/utils.h"
#include "../../logger/logger.h"

/* Decleration */

/**
 * @brief Add a new symbol to the symbols structure.
 *
 * This function adds a new symbol to the symbols structure after performing validity checks.
 * It allocates memory for the new symbol's name and properties, then adds it to the structure.
 *
 * @param symbols A pointer to the Symbols structure.
 * @param name The name of the new symbol.
 * @param type The type of the new symbol.
 * @param decimal_address The decimal address associated with the symbol.
 * @param line_in_file The line number in the input file where the symbol is located.
 */
void addNewSymbol(Symbols *, char *, SYMBOL_TYPE, int, int);
/**
 * @brief Check if a new symbol is valid before adding.
 *
 * This function checks whether a new symbol is valid for addition to the symbols structure.
 * It ensures that the symbol's name is not a duplicate and that it adheres to naming conventions.
 *
 * @param symbols A pointer to the Symbols structure.
 * @param name The name of the new symbol.
 * @param type The type of the new symbol.
 * @param line_in_file The line number in the input file where the symbol is located.
 * @return True if the new symbol is valid, otherwise false.
 */
bool newSymbolIsValid(Symbols *, char *, SYMBOL_TYPE, int);

/* Implementation */

bool newSymbolIsValid(Symbols *symbols, char *name, SYMBOL_TYPE type, int line_in_file)
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
            logError("line %d: Duplicate symbols '%s' found", line_in_file, name);
            return false;
        }

        /* Check for names with spaces, which are invalid for symbols */
        if (has_spaces)
        {
            logError("line %d: Name '%s' has spaces", line_in_file, name);
            return false;
        }
    }

    return true;
}

void addNewSymbol(Symbols *symbols, char *name, SYMBOL_TYPE type, int decimal_address, int line_in_file)
{
    Symbol *new_symbol = NULL;
    bool symbol_is_valid = newSymbolIsValid(symbols, name, type, line_in_file);

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
    new_symbol->decimal_address = decimal_address;

    /* Check if the new symbol is valid before adding */
    if (symbol_is_valid)
        new_symbol->type = type;
    else
        new_symbol->type = INVALID_SYMBOL;
}

void insertNewSymbol(char *line, Symbols *symbols, size_t symbol_length, SYMBOL_TYPE type, int line_in_file)
{
    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    /* Slice the string starting from 'symbol_length' */
    line = slice(line, symbol_length);

    /* Trim leading and trailing spaces from the sliced string */
    line = trim(line);

    /* Add the new symbol to the Symbols structure */
    addNewSymbol(symbols, line, type, 0, line_in_file);
}

void insertNewLabel(char *line, Symbols *symbols, int decimal_address, int line_in_file)
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
    addNewSymbol(symbols, label, LABEL, decimal_address, line_in_file);
}

bool has_invalid_symbol(Symbols symbols)
{
    int i;
    for (i = 0; i < symbols.amount; i++)
        if (symbols.array[i]->type == INVALID_SYMBOL)
            return true;
    return false;
}

void free_symbols(Symbols *symbols)
{
    int i;
    Symbol *symbol;

    /* Free memory for each individual Symbol structure */
    for (i = 0; i < symbols->amount; i++)
    {
        symbol = symbols->array[i];

        /* Free memory for the symbol name */
        free(symbol->name);

        /* Free the memory for the Symbol structure itself */
        free(symbol);
    }

    /* Free memory for the array of Symbol pointers */
    free(symbols->array);
}
