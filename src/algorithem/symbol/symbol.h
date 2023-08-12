#ifndef REFERENCE_H
#define REFERENCE_H

#include "../../utils/utils.h"

typedef enum
{
    EXTERN,
    ENTRY,
    LABEL
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
 * @param line_in_file The line number in the source file where the symbol is declared.
 */
void insertNewSymbol(char *, Symbols *, size_t, SYMBOL_TYPE, int);

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
 * @param line_in_file The line number in the source file where the symbol is declared.
 */
void insertNewLabel(char *, Symbols *, int, int);

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
 * @param line_in_file The line number in the source file where the symbol is declared.
 */
void addNewSymbol(Symbols *, char *, SYMBOL_TYPE, int, int);

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
 * @param line_in_file The line number in the source file where the symbol is declared.
 * @return Returns true if the new symbol is valid, false otherwise.
 */
bool newSymbolIsValid(Symbols *, char *, SYMBOL_TYPE, int);

#endif