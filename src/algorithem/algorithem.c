#include <stdio.h>
#include "../utils/utils.h"
#include "reference.h"
#include "instruction.h"

void insertNewExtern(char *, Reference);
void insertNewEntry(char *, Reference);
void insertNewInstruction(char *, Instructions);

Instructions destructureFile(FILE *file)
{
    char line[LINE_LENGTH];
    Reference externs, entries;
    Instructions instructions;

    /* init the externs, entries and instructions */
    externs.array = (char **)malloc(sizeof(char *));
    externs.amount = 0;

    entries.array = (char **)malloc(sizeof(char *));
    entries.amount = 0;

    instructions.array = (Instruction **)malloc(sizeof(Instruction *));
    instructions.amount = 0;

    /* first scan: get all externs, entries and instructions lists */
    while (fgets(line, LINE_LENGTH, file))
    {
        /* if the line contains ".exter"*/
        if (strstr(line, ".extern") != NULL)
            insertNewExtern(line, externs);
        /* if the line contains ".entry"*/
        else if (strstr(line, ".entry") != NULL)
            insertNewEntry(line, entries);
        /* The line contains an instruction */
        else
            insertNewInstruction(line, instructions);
    }

    /* Reset the file pointer to the beginning of the file */
    fseek(file, 0, SEEK_SET);

    /* second scan (instructions.array): place externs and entries to files and instructions unknown binary */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */
    /* IMPLEMENTATION */

    return instructions;
}

void insertNewExtern(char *line, Reference externs) {}
void insertNewEntry(char *line, Reference entries) {}
void insertNewInstruction(char *line, Instructions instructions) {}