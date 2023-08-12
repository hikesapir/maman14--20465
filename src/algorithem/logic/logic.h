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

#endif