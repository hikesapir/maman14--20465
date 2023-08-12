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
 * @brief Determine the command type based on the input line.
 *
 * This function identifies the command type based on the command name in the input line.
 * It searches for a match in the command_definition array and updates the command_type accordingly.
 *
 * @param command_type Pointer to store the detected command type.
 * @param line The input line to analyze.
 * @param command_definition An array of CMD_Definition representing command definitions.
 * @param line_in_file The line number in the input file where the command is located.
 * @return Pointer to the remaining part of the line after the command name,
 *         or NULL if the command is undefined.
 */
char *get_command_type(Command_Type *, char *, CMD_Definition[], int);

/**
 * @brief Parse the string command arguments from the input line.
 *
 * This function populates the Arguments structure for a STRING command.
 *
 * @param command Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 * @param line_in_file The line number in the input file where the command is located.
 */
void get_string_command_arguments(Command *, char *, int);

/**
 * @brief Parse and extract the arguments for a command.
 *
 * This function tokenizes the input line to separate individual arguments
 * and stores them in the Arguments structure. It determines the argument type
 * and allocates memory to store the argument name.
 *
 * @param arguments Pointer to store the parsed arguments.
 * @param line The input line containing the arguments.
 */
void get_command_arguments(Arguments *, char *);

/**
 * @brief Check if command arguments are valid.
 *
 * This function checks if the given command's arguments are valid according to
 * the specified command definitions. It ensures the correct number of arguments
 * and their types match the expected format.
 *
 * @param command Pointer to the command structure.
 * @param command_definition An array of CMD_Definition representing command definitions.
 * @param line_in_file The line number in the input file where the command is located.
 * @return True if the arguments are valid, otherwise false.
 */
bool arguments_is_valid(Command *, CMD_Definition[], int);

/**
 * @brief Advance the decimal address based on the command's arguments.
 *
 * This function updates the decimal address based on the number and types of
 * arguments in the given command. It considers both registers and non-register
 * arguments to determine the appropriate address advancement.
 *
 * @param command Pointer to the command structure.
 * @param decimal_address A pointer to the decimal address value.
 */
void advance_decimal_adress(Command *, int *);

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

/**
 * @brief Set the binary representation for a command.
 *
 * This function sets the binary representation for the given command based on its type and arguments.
 *
 * @param command Pointer to the command structure.
 */
void set_command_binary(Command *);

#endif