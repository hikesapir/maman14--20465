#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/utils.h"

#define LOGGER_PATH "logs/logger.log"

void printFormatted(const char *format, va_list args)
{
    FILE *loggerFile;

    /* Get current time */
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    /* Format timestamp */
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%d/%m/%y %H:%M:%S", local_time);

    /* Print timestamp and log message to console */
    printf("[%s] ", timestamp);
    vprintf(format, args);

    /* Print timestamp and log message to log file */
    createDirIfNotExists(LOGGER_FOLDER);

    loggerFile = fopen(LOGGER_PATH, "a");
    if (loggerFile != NULL)
    {

        fprintf(loggerFile, "[%s] ", timestamp);
        vfprintf(loggerFile, format, args);

        fclose(loggerFile);
    }
}

void logInfo(const char *info, ...)
{
    va_list args;
    char *fullMessage = (char *)malloc(strlen(info) + 6 * sizeof(char));

    strcpy(fullMessage, "INFO: "); /* Copy INFO into fullMessage */
    strcat(fullMessage, info);     /* Add the error to fullMessage */
    strcat(fullMessage, "\n");     /* Add the \n to fullMessage */

    va_start(args, info);
    printFormatted(fullMessage, args);
    va_end(args);

    free(fullMessage);
}

void logError(const char *error, ...)
{
    va_list args;
    char *fullMessage = (char *)malloc(strlen(error) + 6 * sizeof(char));

    strcpy(fullMessage, "ERROR: "); /* Copy ERROR into fullMessage */
    strcat(fullMessage, error);     /* Add the error to fullMessage */
    strcat(fullMessage, "\n");      /* Add the \n to fullMessage */

    va_start(args, error);
    printFormatted(fullMessage, args);
    va_end(args);

    free(fullMessage);
}
