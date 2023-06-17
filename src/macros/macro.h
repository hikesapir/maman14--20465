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

/*
This function takes a .as file and stripping it out of its macros
The stripping process includes removing the macro lines,
And planting them in the correct places.

Input:
    FILE *: a pointer to the FILE that we want to strip of macros
    char *: a pointer to the new stripped file name

Output:
    returns a pointer to a new .am new FILE containing the stripped code
*/
FILE *stripMacros(FILE *, char *);

#endif