#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#define LOGGER_PATH "logs/logger.log"

void createLogsDirIfNotExists()
{
    const char *dirname = "logs";
    struct stat st;
    if (stat(dirname, &st) == -1)
        mkdir(dirname);
}

void printFormatted(const char *format, FILE *file, va_list args)
{
    /* Get current time */
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    /* Format timestamp */
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);

    /* Print timestamp and log message to console */
    printf("[%s] ", timestamp);
    vprintf(format, args);

    /* Print timestamp and log message to log file */
    if (file != NULL)
    {
        fprintf(file, "[%s] ", timestamp);
        vfprintf(file, format, args);
        fclose(file);
    }
}

void logInfo(const char *info, ...)
{
    FILE *infoFile;
    va_list args;

    createLogsDirIfNotExists();

    infoFile = fopen(LOGGER_PATH, "a");

    va_start(args, info);
    printFormatted(info, infoFile, args);
    va_end(args);
}

void logError(const char *error, ...)
{
    FILE *errorFile;
    va_list args;

    createLogsDirIfNotExists();

    errorFile = fopen(LOGGER_PATH, "a");

    va_start(args, error);
    printFormatted(error, errorFile, args);
    va_end(args);
}
