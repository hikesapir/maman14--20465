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

#endif