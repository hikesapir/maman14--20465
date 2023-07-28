#ifndef LABEL_H
#define LABEL_H

typedef struct label
{
    int decimal_address;
    char *name;
    
} Label;

typedef struct labels
{
    Label **array;
    int amount;
} Labels;

#endif