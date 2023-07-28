#include <sys/stat.h>
#include <ctype.h>
#include <string.h>

#include "utils.h"

void createDirIfNotExists(const char *dirname)
{
    struct stat st;

    /* Check if the directory exists by trying to get its status */
    if (stat(dirname, &st) == -1)
        mkdir(dirname); /* If the directory does not exist, create it */
}

char *slice(char *str, int start)
{
    /* Copy the substring of 'str' starting from 'start' into a newly allocated memory */
    return strdup(str + start);
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