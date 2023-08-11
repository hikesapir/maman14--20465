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

typedef enum
{
    ALL = 1,
    ALL_BUT_STATIC = 2,
    VAR = 3,
    STAT = 4
} Argument_Type_Group;

typedef struct cmd_definition
{
    char *name;
    Command_Type type;
    int args_count;
    Argument_Type_Group Destination_Operand_Types;
    Argument_Type_Group Source_Operand_Types;
} CMD_Definition;

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
void insertNewCommand(char *, Commands *, int *, CMD_Definition[]);

/**
 * Determine the command type based on the input line.
 *
 * @param command_type Pointer to store the detected command type.
 * @param line The input line to analyze.
 * @return Pointer to the remaining part of the line after the command name,
 *         or NULL if the command is undefined.
 */
char *get_command_type(Command_Type *, char *, CMD_Definition[]);

/**
 * Parse the command arguments from the input line.
 *
 * @param arguments Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 */
void get_command_arguments(Arguments *, char *);

/**
 * Parse the string command arguments from the input line.
 *
 * @param command Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 */
void get_string_command_arguments(Command *, char *);

bool arguments_is_valid(Command *, CMD_Definition[]);

void advance_decimal_adress(Command *, int *, CMD_Definition[]);

#endif