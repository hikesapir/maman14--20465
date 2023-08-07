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
    int decimal_address;
    int binary_represnt;
    Argument_Type type;
} Argument;

typedef struct arguments
{
    int amount;
    Argument **arr;
} Arguments;

typedef struct command
{
    Command_Type command_type;
    int decimal_address;
    int binary_represnt;
    Arguments arguments;
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
    {"not", NOT, 1, ALL_BUT_STATIC, NULL},
    {"clr", CLR, 1, ALL_BUT_STATIC, NULL},
    {"lea", LEA, 2, ALL_BUT_STATIC, VAR},
    {"inc", INC, 1, ALL_BUT_STATIC, NULL},
    {"dec", DEC, 1, ALL_BUT_STATIC, NULL},
    {"jmp", JMP, 1, ALL_BUT_STATIC, NULL},
    {"bne", BNE, 1, ALL_BUT_STATIC, NULL},
    {"red", RED, 1, ALL_BUT_STATIC, NULL},
    {"prn", PRN, 1, ALL, NULL},
    {"jsr", JSR, 1, ALL_BUT_STATIC, NULL},
    {"rts", RTS, 0, NULL, NULL},
    {"stop", STOP, 0, NULL, NULL},
    {".string", STRING, -1, STAT, STAT},
    {".data", DATA, -1, STAT, STAT}};

/**
 * Inserts a new command into the Commands structure based on the given line and address.
 *
 * This function processes the given line to extract and parse the command information.
 * It then adds the parsed command to the Commands structure with the provided address.
 *
 * @param line The line containing the command information to be parsed and added.
 * @param commands The Commands structure to which the new command will be added.
 * @param decimal_address The decimal address associated with the command.
 */
void insertNewCommand(char *, Commands *, int *);

/**
 * Determine the command type based on the input line.
 *
 * @param command_type Pointer to store the detected command type.
 * @param line The input line to analyze.
 * @return Pointer to the remaining part of the line after the command name,
 *         or NULL if the command is undefined.
 */
char *get_command_type(Command_Type *, char *);

/**
 * Parse the command arguments from the input line.
 *
 * @param arguments Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 */
void get_command_arguments(Arguments *, char *);

void print_arguments(Arguments);
#endif