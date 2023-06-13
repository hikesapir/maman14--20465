#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>

void createLogsDirIfNotExists()
{
    const char *dirname = "logs";
    struct stat st;
    if (stat(dirname, &st) == -1)
        mkdir(dirname);
}

void printFormatted(const char *format, FILE *file, const char *logMessage, ...)
{
    /* Print to console */
    va_list args;
    va_start(args, logMessage);
    vprintf(format, args);
    va_end(args);

    /* Print to log file */
    if (file != NULL)
    {
        va_list fileArgs;
        va_start(fileArgs, logMessage);
        vfprintf(file, format, fileArgs);
        va_end(fileArgs);
        fclose(file);
    }
}

void logInfo(const char *info, ...)
{
    FILE *infoFile;
    va_list args;

    createLogsDirIfNotExists();

    infoFile = fopen("logs/info.log", "a");

    va_start(args, info);
    printFormatted(info, infoFile, args);
    va_end(args);
}

void logError(const char *error, ...)
{
    FILE *errorFile;
    va_list args;

    createLogsDirIfNotExists();

    errorFile = fopen("logs/error.log", "a");

    va_start(args, error);
    printFormatted(error, errorFile, args);
    va_end(args);
}
