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

void printFormatted(const char *format, FILE *file, va_list args)
{
    /* Print to console */
    vprintf(format, args);

    /* Print to log file */
    if (file != NULL)
    {
        vfprintf(file, format, args);
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
