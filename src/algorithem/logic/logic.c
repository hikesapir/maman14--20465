#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "logic.h"
#include "../../utils/utils.h"
#include "../symbol/symbol.h"
#include "../command/command.h"

void destructureFile(FILE *file, Symbols *symbols, Commands *commands)
{
    char line[LINE_LENGTH], *colon_ptr;          /* Buffer to store each line of the input file */
    int decimal_address = 100, line_in_file = 1; /* Starting address for commands */

    CMD_Definition command_definition[] = {
        {"mov", MOV, 2},
        {"cmp", CMP, 2},
        {"add", ADD, 2},
        {"sub", SUB, 2},
        {"not", NOT, 1},
        {"clr", CLR, 1},
        {"lea", LEA, 2},
        {"inc", INC, 1},
        {"dec", DEC, 1},
        {"jmp", JMP, 1},
        {"bne", BNE, 1},
        {"red", RED, 1},
        {"prn", PRN, 1},
        {"jsr", JSR, 1},
        {"rts", RTS, 0},
        {"stop", STOP, 0},
        {".string", STRING, 1},
        {".data", DATA, -1}};

    /* Initialize the symbols and commands structures */
    symbols->array = (Symbol **)malloc(sizeof(Symbol *));
    symbols->amount = 0;

    commands->array = (Command **)malloc(sizeof(Command *));
    commands->amount = 0;

    /* Reset the file pointer to the beginning of the file */
    rewind(file);

    /* First scan: Get all externs, entries, labels, and commands */
    while (fgets(line, LINE_LENGTH, file))
    {
        /* If the line contains ".extern" */
        if (strstr(line, ".extern") != NULL)
            insertNewSymbol(line, &symbols, strlen(".extern\0"), EXTERN, line_in_file);

        /* If the line contains ".entry" */
        else if (strstr(line, ".entry") != NULL)
            insertNewSymbol(line, &symbols, strlen(".entry\0"), ENTRY, line_in_file);

        /* If the line contains a label (indicated by a colon) */
        else if (strstr(line, ":") != NULL)
        {
            /* Insert the new label symbol and the associated command */
            insertNewLabel(line, &symbols, decimal_address, line_in_file);

            colon_ptr = strstr(line, ":");

            insertNewCommand(colon_ptr + 1, &commands, &decimal_address, command_definition, line_in_file);
        }

        /* If the line contains an instruction or a command */
        else
            insertNewCommand(line, &commands, &decimal_address, command_definition, line_in_file);

        line_in_file += 1;
    }
}
