#ifndef REFERENCE_H
#define REFERENCE_H

typedef enum
{
    EXTERN,
    ENTRY,
    LABEL
} SYMBOL_TYPE;

typedef struct symbol
{
    int decimal_address;
    char *name;
    SYMBOL_TYPE type;
} Symbol;

typedef struct symbols
{
    Symbol **array;
    int amount;
} Symbols;

#endif