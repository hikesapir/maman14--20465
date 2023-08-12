#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>

typedef struct macro
{
    char *name;
    char **content;
    int amountOfLines;
} Macro;

typedef struct macros
{
    Macro **array;
    int amountOfMacros;
} Macros;

/**
 * @brief Strips macros from a source file and returns a new stripped file.
 *
 * This function takes a source file and strips it of its macros. The stripping process
 * involves removing macro lines and replacing them in the correct places within the
 * source code. The extracted macros are collected and stored in a `Macros` structure.
 * The output is a newly created stripped file with the ".am" extension.
 *
 * @param sourceFile A pointer to the source file with macros.
 * @param filePath A pointer to the new stripped file name.
 * @return A pointer to a new FILE containing the stripped code.
 */
FILE *stripMacros(FILE *, char *);

#endif