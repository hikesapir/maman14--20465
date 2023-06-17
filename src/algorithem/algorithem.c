#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "algorithem.h"

void insertNewExtern(char *, Reference);
void insertNewEntry(char *, Reference);
void insertNewInstruction(char *, Instructions);

DestructuredFile destructureFile(FILE *file)
{
    char line[LINE_LENGTH];
    DestructuredFile destructuredFile;
    Reference externs = destructuredFile.externs, entries = destructuredFile.entries;
    Instructions instructions = destructuredFile.instructions;

    /* init the externs, entries and instructions */
    externs.array = (char **)malloc(sizeof(char *));
    externs.amount = 0;

    entries.array = (char **)malloc(sizeof(char *));
    entries.amount = 0;

    instructions.array = (Instruction **)malloc(sizeof(Instruction *));
    instructions.amount = 0;

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

    return destructuredFile;
}

void insertNewExtern(char *line, Reference externs) {}
void insertNewEntry(char *line, Reference entries) {}
void insertNewInstruction(char *line, Instructions instructions) {}