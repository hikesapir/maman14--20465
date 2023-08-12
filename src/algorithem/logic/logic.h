#ifndef LOGIC_H
#define LOGIC_H

#include <stdio.h>
#include "../command/command.h"
#include "../symbol/symbol.h"

/**
 * Destructures the provided file, extracting symbols and commands from it.
 *
 * This function reads the input file line by line and extracts relevant information such as
 * externs, entries, labels, and commands. It then creates an array of Commands containing the
 * parsed command information. The symbols found during the process are stored in the Symbols
 * structure, which keeps track of different symbol types.
 *
 * @param file The FILE pointer to the input file that needs to be destructured.
 * @return A Commands structure containing the array of parsed commands and their amount.
 */
void destructureFile(FILE *, Symbols *, Commands *);

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

#endif