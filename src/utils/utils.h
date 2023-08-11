#ifndef UTILS_H
#define UTILS_H

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

/*
 * Function to create a directory if it does not exist
 *
 * Input:
 *    dirname: a pointer to the name of the directory to be created
 */
void createDirIfNotExists(const char *);

/*
 * Function to slice a string from the specified start index
 *
 * Input:
 *     str: A pointer to the input string from which the slicing should begin
 *     start: The starting index from which the slicing should begin
 *
 * Output:
 *     Returns a pointer to a newly allocated substring, starting from 'start' index.
 *     The caller is responsible for freeing the memory using 'free' when it's no longer needed.
 */
char *slice(char *, int);

/*
 * Function to trim leading and trailing spaces from a string
 *
 * Input:
 *    str: a pointer to the input string to be trimmed
 *
 * Output:
 *    returns a pointer to the modified string, which is the same as the input string.
 *    The function modifies the original string directly, so no memory is dynamically allocated.
 */
char *trim(char *);

bool is_numeric(char *);

int decimalToBinary(int);

#endif /* UTILS_H */
