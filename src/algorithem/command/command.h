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
    MOV,            /* Move data between operands. */
    CMP,            /* Compare two operands. */
    ADD,            /* Add operands. */
    SUB,            /* Subtract operands. */
    NOT,            /* Perform bitwise NOT operation on an operand. */
    CLR,            /* Clear (zero out) an operand. */
    LEA,            /* Load effective address. */
    INC,            /* Increment operand value. */
    DEC,            /* Decrement operand value. */
    JMP,            /* Unconditional jump. */
    BNE,            /* Branch if not equal. */
    RED,            /* Read input to operand. */
    PRN,            /* Print operand value. */
    JSR,            /* Jump to subroutine. */
    RTS,            /* Return from subroutine. */
    STOP,           /* Halt program execution. */
    STRING,         /* Define a string in memory. */
    DATA,           /* Define data in memory. */
    INVALID_COMMAND /* Invalid or undefined command. */
} Command_Type;

/**
 * Structure representing an argument for a command.
 */
typedef struct argument
{
    char *name;                    /* Name of the argument. */
    int decimal_address;           /* Decimal address of the argument. */
    int binary_representation[12]; /* Binary representation of the argument. */
    Argument_Type type;            /* Type of the argument. */
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
    Command_Type command_type;     /* Type of the command. */
    int decimal_address;           /* Decimal address of the command. */
    int binary_representation[12]; /* Binary representation of the command. */
    Arguments arguments;           /* List of arguments for the command. */
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
 * @brief Inserts a new command into the commands structure.
 *
 * This function takes a line representing a new command, along with various other parameters, and inserts
 * the command into the commands structure. It identifies the command type, trims the line, processes
 * arguments, validates them, updates the decimal address, and sets the command's binary representation.
 *
 * @param line The line containing the new command.
 * @param commands A pointer to the commands structure.
 * @param decimal_address A pointer to the decimal address value.
 * @param command_definition An array of CMD_Definition structs defining command details.
 * @param line_in_file The line number in the input file.
 */
void insertNewCommand(char *, Commands *, int *, CMD_Definition[], int);

/**
 * @brief Check if any command in the structure is invalid.
 *
 * This function checks if any command within the Commands structure is marked as invalid.
 *
 * @param commands The Commands structure containing the commands.
 * @return True if any command is invalid, otherwise false.
 */
bool has_invalid_command(Commands);

/**
 * @brief Free memory allocated for commands and their arguments.
 *
 * This function frees the memory allocated for the Commands structure,
 * including individual Command structures and their arguments.
 *
 * @param commands A pointer to the Commands structure.
 */
void free_commands(Commands *);

#endif