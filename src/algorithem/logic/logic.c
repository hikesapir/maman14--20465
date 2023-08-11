#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "logic.h"
#include "../../utils/utils.h"
#include "../symbol/symbol.h"
#include "../command/command.h"

Commands destructureFile(FILE *file)
{
    char line[LINE_LENGTH], *colon_ptr; /* Buffer to store each line of the input file */
    int decimal_address = 100;          /* Starting address for commands */
    Symbols symbols;                    /* Structure to store different symbols found in the file */
    Commands commands;                  /* Structure to store the parsed commands from the file */

    CMD_Definition command_definition[] = {
        {"mov", MOV, 2, ALL_BUT_STATIC, ALL},
        {"cmp", CMP, 2, ALL, ALL},
        {"add", ADD, 2, ALL_BUT_STATIC, ALL},
        {"sub", SUB, 2, ALL_BUT_STATIC, ALL},
        {"not", NOT, 1, ALL_BUT_STATIC, 0},
        {"clr", CLR, 1, ALL_BUT_STATIC, 0},
        {"lea", LEA, 2, ALL_BUT_STATIC, VAR},
        {"inc", INC, 1, ALL_BUT_STATIC, 0},
        {"dec", DEC, 1, ALL_BUT_STATIC, 0},
        {"jmp", JMP, 1, ALL_BUT_STATIC, 0},
        {"bne", BNE, 1, ALL_BUT_STATIC, 0},
        {"red", RED, 1, ALL_BUT_STATIC, 0},
        {"prn", PRN, 1, ALL, 0},
        {"jsr", JSR, 1, ALL_BUT_STATIC, 0},
        {"rts", RTS, 0, 0, 0},
        {"stop", STOP, 0, 0, 0},
        {".string", STRING, -1, STAT, STAT},
        {".data", DATA, -1, STAT, STAT}};

    /* Initialize the symbols and commands structures */
    symbols.array = (Symbol **)malloc(sizeof(Symbol *));
    symbols.amount = 0;

    commands.array = (Command **)malloc(sizeof(Command *));
    commands.amount = 0;

    /* Reset the file pointer to the beginning of the file */
    rewind(file);

    /* First scan: Get all externs, entries, labels, and commands */
    while (fgets(line, LINE_LENGTH, file))
    {
        /* If the line contains ".extern" */
        if (strstr(line, ".extern") != NULL)
            insertNewSymbol(line, &symbols, strlen(".extern\0"), EXTERN);

        /* If the line contains ".entry" */
        else if (strstr(line, ".entry") != NULL)
            insertNewSymbol(line, &symbols, strlen(".entry\0"), ENTRY);

        /* If the line contains a label (indicated by a colon) */
        else if (strstr(line, ":") != NULL)
        {
            /* Insert the new label symbol and the associated command */
            insertNewLabel(line, &symbols, decimal_address);

            colon_ptr = strstr(line, ":");

            insertNewCommand(colon_ptr + 1, &commands, &decimal_address, command_definition);
        }

        /* If the line contains an instruction or a command */
        else
            insertNewCommand(line, &commands, &decimal_address, command_definition);
    }

    /* Reset the file pointer to the beginning of the file */
    rewind(file);

    /* second scan (commands.array): place externs and entries to files and commands unknown binary */

    /* Do Not Create the files if there is errors */

    /* For each command */
    /* if there is an unknown variable we will change the command type to INVALID */
    /* write command as binary */

    /* 2 files () */

    /* write as base 64 from binary */
    /* Header will bae (amount of command lines, amount of variable lines) */

    return commands;
}
