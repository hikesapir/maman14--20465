#ifndef LOGIC_H
#define LOGIC_H

#include <stdio.h>
#include "../command/command.h"
#include "../symbol/symbol.h"

/**
 * @brief Destructures a given file into symbols and commands.
 *
 * This function takes a file pointer, symbols, and commands structures as input, and populates them with
 * the relevant information from the provided file. It reads the file line by line, identifying externs,
 * entries, labels, and commands, and associates appropriate data with them.
 *
 * @param file A pointer to the input file.
 * @param symbols A pointer to the symbols structure.
 * @param commands A pointer to the commands structure.
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