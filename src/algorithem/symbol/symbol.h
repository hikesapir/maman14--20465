#ifndef REFERENCE_H
#define REFERENCE_H

#include "../../utils/utils.h"

typedef enum
{
    EXTERN,
    ENTRY,
    LABEL,
    INVALID_SYMBOL
} SYMBOL_TYPE;

typedef struct symbol
{
    int decimal_address;
    char *name;
    SYMBOL_TYPE type;
} Symbol;

typedef struct symbols
{
    Symbol **array;
    int amount;
} Symbols;

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
void insertNewSymbol(char *, Symbols *, size_t, SYMBOL_TYPE, int);

/**
 * @brief Insert a new label into the symbols structure.
 *
 * This function extracts a new label from the input line and inserts it into the symbols structure.
 *
 * @param line The input line containing the new label.
 * @param symbols A pointer to the Symbols structure.
 * @param decimal_address The decimal address associated with the label.
 * @param line_in_file The line number in the input file where the label is located.
 */
void insertNewLabel(char *, Symbols *, int, int);

/**
 * @brief Check if any symbol in the structure is invalid.
 *
 * This function checks if any symbol within the Symbols structure is marked as invalid.
 *
 * @param symbols The Symbols structure containing the symbols.
 * @return True if any symbol is invalid, otherwise false.
 */
bool has_invalid_symbol(Symbols);

/**
 * @brief Free memory allocated for symbols and their properties.
 *
 * This function frees the memory allocated for the Symbols structure,
 * including individual Symbol structures and their names.
 *
 * @param symbols A pointer to the Symbols structure.
 */
void free_symbols(Symbols *);

#endif