#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

/* Constants */
#define LINE_LENGTH 80
#define OUTPUT_FOLDER "output"
#define LOGGER_FOLDER "logs"
#define FILE_SIZE 1024
#define PATH_MAX 260
#define ARGUMENTS_DELIMITER ","
#define CMD_DEFINITIONS_AMOUNT 18

/* Typedefs */
typedef char line[LINE_LENGTH];

typedef enum
{
    true = 1,
    false = 0
} bool;

/**
 * @brief Creates a directory if it does not exist.
 *
 * This function checks if the specified directory exists by attempting to get its status.
 * If the directory does not exist, it creates it.
 *
 * @param dirname The name of the directory to create.
 */
void createDirIfNotExists(const char *);

/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * This function removes leading and trailing whitespace characters (including spaces, tabs, etc.)
 * from the given string. It modifies the string in place and returns a pointer to the modified string.
 *
 * @param str The string to trim.
 * @return A pointer to the trimmed string.
 */
char *trim(char *);

/**
 * @brief Checks if a string represents a numeric value.
 *
 * This function determines whether a given string represents a valid numeric value.
 *
 * @param str The string to check.
 * @return `true` if the string represents a numeric value, otherwise `false`.
 */
bool is_numeric(char *);

/**
 * @brief Converts an integer to its binary representation.
 *
 * This function converts a given integer to its binary representation as an integer.
 *
 * @param n The integer to convert.
 * @return The binary representation of the integer.
 */
int int_to_binary(int);

/**
 * @brief Create and open an output file for writing.
 *
 * This function creates and opens an output file for writing in the specified output folder.
 * The output file's name is constructed by combining the provided file name, its basename, and the given file extension.
 *
 * @param file_name The base name of the output file.
 * @param file_extention The extension to be added to the output file name.
 * @return A pointer to the opened FILE structure, or NULL if an error occurred.
 */
FILE *create_output_file(char *, const char *);

#endif /* UTILS_H */
