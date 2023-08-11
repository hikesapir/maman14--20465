#ifndef COMMAND_H
#define COMMAND_H
#include "../../utils/utils.h"

/**
 * Enumeration representing different types of arguments for a command.
 */
typedef enum
{
    STATIC = 1,   /* Represents a static argument. */
    VARIABLE = 3, /* Represents a variable argument. */
    REGISTER = 5  /* Represents a register argument. */
} Argument_Type;

/**
 * Enumeration representing different types of commands.
 */
typedef enum
{
    MOV,    /* Move data between operands. */
    CMP,    /* Compare two operands. */
    ADD,    /* Add operands. */
    SUB,    /* Subtract operands. */
    NOT,    /* Perform bitwise NOT operation on an operand. */
    CLR,    /* Clear (zero out) an operand. */
    LEA,    /* Load effective address. */
    INC,    /* Increment operand value. */
    DEC,    /* Decrement operand value. */
    JMP,    /* Unconditional jump. */
    BNE,    /* Branch if not equal. */
    RED,    /* Read input to operand. */
    PRN,    /* Print operand value. */
    JSR,    /* Jump to subroutine. */
    RTS,    /* Return from subroutine. */
    STOP,   /* Halt program execution. */
    STRING, /* Define a string in memory. */
    DATA,   /* Define data in memory. */
    INVALID /* Invalid or undefined command. */
} Command_Type;

/**
 * Structure representing an argument for a command.
 */
typedef struct argument
{
    char *name;                /* Name of the argument. */
    int decimal_address;       /* Decimal address of the argument. */
    int binary_representation; /* Binary representation of the argument. */
    Argument_Type type;        /* Type of the argument. */
} Argument;

/**
 * Structure representing a list of arguments for a command.
 */
typedef struct arguments
{
    int amount;     /* Number of arguments in the list. */
    Argument **arr; /* Array of Argument pointers. */
} Arguments;

/**
 * Structure representing a command.
 */
typedef struct command
{
    Command_Type command_type; /* Type of the command. */
    int decimal_address;       /* Decimal address of the command. */
    int binary_representation; /* Binary representation of the command. */
    Arguments arguments;       /* List of arguments for the command. */
} Command;

/**
 * Structure representing a list of commands.
 */
typedef struct commands
{
    Command **array; /* Array of Command pointers. */
    int amount;      /* Number of commands in the list. */
} Commands;

/**
 * Structure representing the definition of a command.
 */
typedef struct cmd_definition
{
    char *name;        /* Name of the command. */
    Command_Type type; /* Type of the command. */
    int args_count;    /* Number of expected arguments. */
} CMD_Definition;

/**
 * Insert a new command into the Commands structure based on the given line and address.
 *
 * This function processes the given line to extract and parse the command information.
 * It then adds the parsed command to the Commands structure with the provided address.
 *
 * @param line The line containing the command information to be parsed and added.
 * @param commands The Commands structure to which the new command will be added.
 * @param decimal_address The decimal address associated with the command.
 * @param command_definition An array of CMD_Definition representing command definitions.
 */
void insertNewCommand(char *, Commands *, int *, CMD_Definition[]);

/**
 * Determine the command type based on the input line.
 *
 * @param command_type Pointer to store the detected command type.
 * @param line The input line to analyze.
 * @param command_definition An array of CMD_Definition representing command definitions.
 * @return Pointer to the remaining part of the line after the command name,
 *         or NULL if the command is undefined.
 */
char *get_command_type(Command_Type *, char *, CMD_Definition[]);

/**
 * Parse the string command arguments from the input line.
 *
 * This function populates the Arguments structure for a STRING command.
 *
 * @param command Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 */
void get_string_command_arguments(Command *, char *);

/*
 * Parses and extracts the arguments for a command.
 *
 * This function tokenizes the input line to separate individual arguments
 * based on the specified delimiter and stores them in the Arguments structure.
 * For each argument, it determines its type (REGISTER, STATIC, or VARIABLE)
 * and allocates memory to store the argument name.
 *
 * @param arguments Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 */
void get_command_arguments(Arguments *, char *);

/**
 * Check if the arguments for a given command are valid.
 *
 * @param command Pointer to the command to check.
 * @param command_definition An array of CMD_Definition representing command definitions.
 * @return True if the arguments are valid, false otherwise.
 */
bool arguments_is_valid(Command *, CMD_Definition[]);

/**
 * Advance the decimal address based on the size of a command and its arguments.
 *
 * @param command Pointer to the command to advance the address for.
 * @param decimal_address Pointer to the current decimal address, which will be updated.
 */
void advance_decimal_adress(Command *, int *);

#endif