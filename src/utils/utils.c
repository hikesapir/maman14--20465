#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>

#include "utils.h"
#include "../logger/logger.h"

void createDirIfNotExists(const char *dirname)
{
    struct stat st;

    /* Check if the directory exists by trying to get its status */
    if (stat(dirname, &st) == -1)
        mkdir(dirname, 0777); /* If the directory does not exist, create it */
}

char *trim(char *str)
{
    char *end;

    /* Trim leading spaces by moving the 'str' pointer until a non-space character is found */
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) /* If the string is empty or contains only spaces, return it as it is */
        return str;

    /* Trim trailing spaces by moving the 'end' pointer backward until a non-space character is found */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    /* Write a new null terminator to truncate the string at the last non-space character */
    end[1] = '\0';

    return str;
}

bool is_numeric(char *str)
{
    int len;
    float ignore;
    int ret = sscanf(str, "%f%n", &ignore, &len);
    return ret && len == strlen(str);
}

int int_to_binary(int n)
{
    int binary = 0;
    int digit, place = 1;

    if (n < 0)
        n = -n;

    while (n > 0)
    {
        digit = n % 2;
        binary += digit * place;
        place *= 10;
        n /= 2;
    }

    return binary;
}

FILE *create_output_file(char *file_name, const char *file_extention)
{
    FILE *file_ptr;
    char fullFileName[PATH_MAX], file_output_folder[PATH_MAX];

    createDirIfNotExists(OUTPUT_FOLDER);

    strcpy(file_output_folder, OUTPUT_FOLDER);       /* Add output to the fullFileName*/
    strcat(file_output_folder, "/");                 /* Add \ to the fullFileName*/
    strcat(file_output_folder, basename(file_name)); /* Copy fileName into fullFileName */

    createDirIfNotExists(file_output_folder);

    strcpy(fullFileName, OUTPUT_FOLDER);       /* Add output to the fullFileName*/
    strcat(fullFileName, "/");                 /* Add \ to the fullFileName*/
    strcat(fullFileName, basename(file_name)); /* Copy fileName into fullFileName */
    strcat(fullFileName, "/");                 /* Add \ to the fullFileName*/
    strcat(fullFileName, basename(file_name)); /* Copy fileName into fullFileName */
    strcat(fullFileName, file_extention);      /* Add the .ent suffix to fullFileName */

    logInfo("oppening %s", fullFileName);
    file_ptr = fopen(fullFileName, "w+"); /* read and write */

    return file_ptr;
}