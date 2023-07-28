#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utils/utils.h"
#include "reference.h"
#include "command.h"
#include "label.h"

void insertNewExtern(char *, Reference);
void insertNewEntry(char *, Reference);
void insertNewInstruction(char *, Commands, int *);
void insertNewLabel(char *, Labels, int);

Commands destructureFile(FILE *file)
{
    char line[LINE_LENGTH];
    int *decimal_address;
    Reference externs, entries;
    Commands commands;
    Labels labels;

    /* init the externs, entries and commands */
    *decimal_address = 100;

    externs.array = (char **)malloc(sizeof(char *));
    externs.amount = 0;

    entries.array = (char **)malloc(sizeof(char *));
    entries.amount = 0;

    commands.array = (Command **)malloc(sizeof(Command *));
    commands.amount = 0;

    labels.array = (Label **)malloc(sizeof(Label *));
    labels.amount = 0;

    /* first scan: get all externs, entries and commands lists */
    while (fgets(line, LINE_LENGTH, file))
    {
        /* if the line contains ".exter"*/
        if (strstr(line, ".extern") != NULL)
            insertNewExtern(line, externs);
        /* if the line contains ".entry"*/
        else if (strstr(line, ".entry") != NULL)
            insertNewEntry(line, entries);
        /* The line contains an instruction */
        else if (strstr(line, ":") != NULL)
        {
            insertNewLabel(line, labels, *decimal_address);
            insertNewInstruction(line, commands, decimal_address);
        }
        /* The line contains an instruction */
        else
            insertNewInstruction(line, commands, decimal_address);
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

void slice(char *str, char *result, int start)
{
    strcpy(result, str + start);
}

char *trim(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

void insertNewExtern(char *line, Reference externs)
{
    char *extern_name;

    line = trim(line);

    int start = strlen(".extern\0");

    slice(line, extern_name, start);

    extern_name = trim(extern_name);

    externs.amount += 1;

    externs.array = (char **)realloc(externs.array, externs.amount * sizeof(char *));
    externs.array[externs.amount - 1] = malloc(strlen(extern_name));

    /* save the line into the macro's content */
    strcpy(externs.array[externs.amount - 1], extern_name);
}
void insertNewEntry(char *line, Reference entries) {}
void insertNewLabel(char *line, Labels labels, int decimal_address) {}
void insertNewInstruction(char *line, Commands commands, int *decimal_address) {}
