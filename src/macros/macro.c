#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"
#include "../utils/utils.h"
#include "../logger/logger.h"

FILE *createStrippedFile(char *);
Macros stripMacrosFromSource(FILE *, FILE *);
void replaceMacrosWithContent(FILE *, Macros);
char *getMacroName(char *);

FILE *stripMacros(FILE *sourceFile, char *filePath)
{
    /* Create the new stripped file */
    FILE *stripped_file = createStrippedFile(filePath);

    /* get all macros before stripping the source file */
    Macros macros = stripMacrosFromSource(sourceFile, stripped_file);

    /* closing the source file (end of usage) */
    fclose(sourceFile);

    /* replace every macro instance with the corresponding lines */
    replaceMacrosWithContent(stripped_file, macros);

    /* free macros resources (end of usage) */
    free(macros.array);

    return stripped_file;
}

FILE *createStrippedFile(char *filePath)
{
    FILE *stripped_file;
    char fullFileName[PATH_MAX];

    createDirIfNotExists(OUTPUT_FOLDER);

    strcpy(fullFileName, OUTPUT_FOLDER);      /* Add output to the fullFileName*/
    strcat(fullFileName, "\\");               /* Add \ to the fullFileName*/
    strcat(fullFileName, basename(filePath)); /* Copy fileName into fullFileName */
    strcat(fullFileName, ".am");              /* Add the .as suffix to fullFileName */

    logInfo("oppening %s\n", fullFileName);
    stripped_file = fopen(fullFileName, "w+"); /* read and write */

    if (stripped_file == NULL)
        logError("Could not open file %s\n", fullFileName);
    else
        logInfo("File opened successfully!\n");

    return stripped_file;
}

Macros stripMacrosFromSource(FILE *sourceFile, FILE *strippedFile)
{
    Macros macros;
    Macro *newMacro;
    char line[LINE_LENGTH];

    /* initiate the macros array */
    macros.array = (Macro **)malloc(sizeof(Macro *));
    /* initiate the macros count */
    macros.amountOfMacros = 0;

    while (fgets(line, LINE_LENGTH, sourceFile))
    {
        /* if the line contains with "mcro" */
        if (strstr(line, "mcro ") != NULL)
        {
            /* create new Macro */
            macros.amountOfMacros += 1;
            macros.array = (Macro **)realloc(macros.array, macros.amountOfMacros * sizeof(Macro *));
            macros.array[macros.amountOfMacros - 1] = malloc(sizeof(Macro));
            newMacro = macros.array[macros.amountOfMacros - 1];

            /* set the macro's name */
            newMacro->name = getMacroName(line);
            /* set the macro's content */
            newMacro->content = (char **)malloc(sizeof(char *));
            /* set the macro's LinesCount */
            newMacro->amountOfLines = 0;

            /* while we did'nt encounter the end of the macro */
            while (strstr(fgets(line, LINE_LENGTH, sourceFile), "endmcro") == NULL)
            {
                /* create new place for the line */
                newMacro->amountOfLines += 1;
                newMacro->content = (char **)realloc(newMacro->content, newMacro->amountOfLines * sizeof(char *));
                newMacro->content[newMacro->amountOfLines - 1] = malloc(sizeof(line));

                /* save the line into the macro's content */
                strcpy(newMacro->content[newMacro->amountOfLines - 1], line);
            }
        }
        else
            /* Non macro & non comment lines => write to the stripped file */
            if (strstr(line, ";") == NULL)
                fprintf(strippedFile, "%s", line);
    }

    return macros;
}

void replaceMacrosWithContent(FILE *strippedFile, Macros macros) {}

char *getMacroName(char *source)
{
    char *target, *locationOfMCRO = strstr(source, "mcro");
    int length = strlen(source), i = 0, j = 0, charactersToMCRO, macroNameLength;

    /* get name length */
    charactersToMCRO = locationOfMCRO - source;
    macroNameLength = length - charactersToMCRO - 5;

    target = malloc((macroNameLength) * sizeof(char));

    /* "mcro <MACRO NAME>\n" */
    for (i = charactersToMCRO + 5, j = 0; i <= (length - 2); i++, j++)
        target[j] = source[i];

    /* assign NULL at the end of string */
    target[j] = '\0';

    return target;
}
