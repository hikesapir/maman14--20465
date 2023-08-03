#ifndef COMMAND_H
#define COMMAND_H

typedef enum
{
    STATIC = 1,
    VARIABLE = 3,
    REGISTER = 5
} Argument_Type;

typedef enum
{
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP,
    STRING,
    DATA,
    INVALID
} Command_Type;

typedef struct argument
{
    char *name;
    Argument_Type type;
} Argument;

typedef struct command
{
    Command_Type cmd;
    Argument **arguments;
} Command;

typedef struct commands
{
    Command **array;
    int amount;
} Commands;

typedef struct cmd_definition
{
    char *name;
    Command_Type type;
    int args_count;
    Argument_Type *Destination_Operand_Types;
    Argument_Type *Source_Operand_Types;
} CMD_Definition;

static Argument_Type ALL[] = {STATIC, VARIABLE, REGISTER};
static Argument_Type ALL_BUT_STATIC[] = {VARIABLE, REGISTER};
static Argument_Type VAR[] = {VARIABLE};
static Argument_Type STAT[] = {STATIC};

static CMD_Definition command_definition[] = {
    {"mov", MOV, 2, ALL_BUT_STATIC, ALL},
    {"cmp", CMP, 2, ALL, ALL},
    {"add", ADD, 2, ALL_BUT_STATIC, ALL},
    {"sub", SUB, 2, ALL_BUT_STATIC, ALL},
    {"not", NOT, 1, ALL_BUT_STATIC},
    {"clr", CLR, 1, ALL_BUT_STATIC},
    {"lea", LEA, 2, ALL_BUT_STATIC, VAR},
    {"inc", INC, 1, ALL_BUT_STATIC},
    {"dec", DEC, 1, ALL_BUT_STATIC},
    {"jmp", JMP, 1, ALL_BUT_STATIC},
    {"bne", BNE, 1, ALL_BUT_STATIC},
    {"red", RED, 1, ALL_BUT_STATIC},
    {"prn", PRN, 1, ALL},
    {"jsr", JSR, 1, ALL_BUT_STATIC},
    {"rts", RTS, 0},
    {"stop", STOP, 0},
    {".string", STRING, -1, STAT},
    {".data", DATA, -1, STAT}};

#endif