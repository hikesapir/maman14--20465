#ifndef ALGORITHEM_H
#define ALGORITHEM_H

#include <stdio.h>
#include "instruction.h"
#include "reference.h"

typedef struct destructuredFile
{
    Instructions instructions;
    Reference externs, entries;
} DestructuredFile;

DestructuredFile destructureFile(FILE *);

#endif