#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utils/utils.h"
#include "reference.h"
#include "command.h"
#include "label.h"
#include "../logger/logger.h"

/*
 * Function to insert a new reference into the Reference structure
 *
 * This function trims leading and trailing spaces from the 'line' and then slices the string
 * starting from 'symbol_length'. It then adds the sliced string into the 'reference.array'.
 * The function also increases the 'reference.amount' to reflect the new number of references.
 * It logs the processed line using the 'logInfo' function.
 *
 * Input:
 *     line: A pointer to the input line containing the reference to be inserted
 *     reference: The Reference structure to which the new reference is to be inserted
 *     symbol_length: The length of the symbol to be sliced and added to the 'reference.array'
 *
 * Note: The 'reference.array' should be initialized appropriately before calling this function.
 *
 * The 'line' pointer is modified during the process, pointing to the remaining part after slicing.
 * The caller should ensure that 'line' points to a valid memory region.
 *
 * The 'reference.array' is updated dynamically, and the caller is responsible for freeing the memory
 * using 'free' when it's no longer needed.
 */
void insertNewReference(char *, Reference, size_t);
void insertNewInstruction(char *, Commands, int *);
void insertNewLabel(char *, Labels, int);

Commands destructureFile(FILE *file)
{
    char line[LINE_LENGTH];
    int decimal_address = 100;
    Reference externs, entries;
    Commands commands;
    Labels labels;

    /* init the externs, entries and commands */

    externs.array = (char **)malloc(sizeof(char *));
    externs.amount = 0;

    entries.array = (char **)malloc(sizeof(char *));
    entries.amount = 0;

    commands.array = (Command **)malloc(sizeof(Command *));
    commands.amount = 0;

    labels.array = (Label **)malloc(sizeof(Label *));
    labels.amount = 0;

    /* Reset the file pointers */
    rewind(file);

    /* first scan: get all externs, entries and commands lists */
    while (fgets(line, LINE_LENGTH, file))
    {
        /* if the line contains ".exter"*/
        if (strstr(line, ".extern") != NULL)
            insertNewReference(line, externs, strlen(".extern\0"));
        /* if the line contains ".entry"*/
        else if (strstr(line, ".entry") != NULL)
            insertNewReference(line, entries, strlen(".entry\0"));
        /* The line contains an instruction */
        else if (strstr(line, ":") != NULL)
        {
            insertNewLabel(line, labels, decimal_address);
            insertNewInstruction(line, commands, &decimal_address);
        }
        /* The line contains an instruction */
        else
            insertNewInstruction(line, commands, &decimal_address);
    }

    /* Reset the file pointer to the beginning of the file */
    fseek(file, 0, SEEK_SET);

    /* second scan (commands.array): place externs and entries to files and commands unknown binary */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */

    return commands;
}

void insertNewReference(char *line, Reference reference, size_t symbol_length)
{
    /* Trim leading and trailing spaces from 'line' */
    line = trim(line);

    /* Slice the string starting from 'symbol_length' */
    line = slice(line, symbol_length);

    /* Trim leading and trailing spaces from the sliced string */
    line = trim(line);

    /* Increment the amount of references */
    reference.amount += 1;

    /* Reallocate memory for the 'reference.array' to accommodate the new reference */
    reference.array = (char **)realloc(reference.array, reference.amount * sizeof(char *));
    reference.array[reference.amount - 1] = malloc(strlen(line));

    /* Save the line into the macro's content */
    strcpy(reference.array[reference.amount - 1], line);
}

void insertNewLabel(char *line, Labels labels, int decimal_address) {}
void insertNewInstruction(char *line, Commands commands, int *decimal_address) {}
